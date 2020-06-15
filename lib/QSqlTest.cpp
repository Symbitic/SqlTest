#include "QSqlTest.h"
#include "QSqlTestCsvFile.h"
#include "QSqlTestJsonFile.h"
#include "QSqlTestDatabase.h"
#include "QSqlTestLogger.h"
#include <QFileInfo>
#include <QDir>
#include <QRegExp>

#include <QUrl>
#include <QDebug>

#define RETURN_ERROR(err)                                                                          \
    d->error = err;                                                                                \
    emit error(err);                                                                               \
    return false;

#define RETURN_SUCCESS()                                                                           \
    if (d->error != SqlTestError::NoError) {                                                       \
        d->error = SqlTestError::NoError;                                                          \
    }                                                                                              \
    return true;

struct QSqlTestPrivate
{
    QSqlTestPrivate()
        : loaded(false)
        , db()
        , json()
        , error()
        , passed(0)
        , failed(0)
        , tests()
        , skipped(0)
    {
    }

    bool loaded;
    QSqlTestDatabase db;
    QSqlTestJsonFile json;
    QString error;
    QString dirName;
    int passed;
    int failed;
    int skipped;
    QList<QSqlTestCase *> tests;
};

static const auto FileNotFound = QLatin1String("Error loading %1");
static const auto TableNotCreated = QLatin1String("Error creating %1: %2");

QSqlTest::QSqlTest(QObject *parent)
    : QObject(parent)
    , d(new QSqlTestPrivate)
{
#if 0
    /* clang-format off */
    const auto errorHandler = [=](const QString &err) {
        d->error = err;
        emit error(err);
    };
    QObject::connect(&d->db, &QSqlTestDatabase::error, errorHandler);
    QObject::connect(&d->json, &QSqlTestJsonFile::error, errorHandler);
    /* clang-format on */
#endif
}

bool QSqlTest::isConnected() const
{
    return d->db.connected();
}

bool QSqlTest::isLoaded() const
{
    return d->loaded;
}

QString QSqlTest::errorString() const
{
    return d->error;
}

int QSqlTest::passed() const
{
    return d->passed;
}

int QSqlTest::failed() const
{
    return d->failed;
}

int QSqlTest::skipped() const
{
    return d->skipped;
}

QList<QSqlTestCase *> QSqlTest::tests() const
{
    return d->tests;
}

void QSqlTest::enableLog(bool enable)
{
    QSqlTestLogger::setLogging(enable);
}

bool QSqlTest::load(const QString &jsonFilename)
{
    bool ret = d->json.load(jsonFilename);
    if (!ret) {
        RETURN_ERROR(FileNotFound.arg(jsonFilename));
    }

    for (const auto testName : d->json.tests().keys()) {
        d->tests.append(new QSqlTestCase(testName));
    }

    d->dirName = QFileInfo(jsonFilename).absolutePath();
    d->loaded = true;

    emit testsChanged();

    RETURN_SUCCESS();
}

bool QSqlTest::connect()
{
    QSqlTestDatabaseSettings settings;
    settings.driver = d->json.driver();
    settings.database = d->json.database();
    settings.username = d->json.username();
    settings.password = d->json.password();
    settings.hostname = d->json.hostname();
    settings.port = d->json.port();

    bool ret = d->db.connect(settings);
    if (!ret) {
        RETURN_ERROR(d->db.lastError());
    }

    RETURN_SUCCESS();
}

bool QSqlTest::createTables()
{
    QSqlTestLogger::print("Creating test tables");

    QHash<QString, QStringList>::const_iterator i;
    const auto tables = d->json.tables();
    for (i = tables.constBegin(); i != tables.constEnd(); ++i) {
        const auto params = i.value().join(", ");
        const auto dropStatement = QString("drop table %1").arg(i.key());
        const auto createStatement = QString("create table %1 (%2)").arg(i.key()).arg(params);

        d->db.exec(dropStatement);

        bool ret = d->db.exec(createStatement);
        if (!ret) {
            // TODO: Better message.
            QSqlTestLogger::print("<red>FAILED!</red>");
            RETURN_ERROR(TableNotCreated.arg(i.key()).arg(d->db.lastError()));
        }
    }

    QSqlTestLogger::print("<green>SUCCESS!</green>");
    RETURN_SUCCESS();
}

bool QSqlTest::startTest(const QString &testName)
{
    QSqlTestCsvFile inputFile;
    QSqlTestCsvFile outputFile;
    bool ret;
    const auto testCase = d->json.tests().value(testName);
    const auto tableName = testCase.tableName;
    auto inputFilename = testCase.input;
    auto outputFilename = testCase.output;
    const auto queryName = testCase.queryName;
    const auto tableNames = testCase.tableNames;
    auto query = d->json.queries().value(queryName);

    for (const auto tableName : tableNames.keys()) {
        const auto before = QString("{{%1}}").arg(tableName);
        const auto after = tableNames.value(tableName);
        query.replace(before, after);
    }

    inputFilename = QDir(d->dirName).filePath(inputFilename);
    outputFilename = QDir(d->dirName).filePath(outputFilename);

    ret = inputFile.load(inputFilename);
    if (!ret) {
        const auto &err = FileNotFound.arg(inputFilename);
        QSqlTestLogger::fail(QString("%1 (%2)").arg(testName).arg(err));
        RETURN_ERROR(err);
    }

    ret = outputFile.load(outputFilename);
    if (!ret) {
        const auto &err = FileNotFound.arg(outputFilename);
        QSqlTestLogger::fail(QString("%1 (%2)").arg(testName).arg(err));
        RETURN_ERROR(err);
    }

    QString columnNames = inputFile.headerData(0).toString();
    QString inserts = "?";
    for (int i = 1; i < inputFile.columnCount(); i++) {
        columnNames.append(", " + inputFile.headerData(i).toString());
        inserts.append(", ?");
    }

    // Do not check return value.
    ret = d->db.exec(QLatin1String("DELETE FROM %1").arg(tableName));

    const auto insertString = QLatin1String("INSERT INTO %1 (%2) VALUES (%3)")
                                      // Temporary testing table.
                                      .arg(tableName)
                                      // CSV column names.
                                      .arg(columnNames)
                                      // Batch insert values.
                                      .arg(inserts);

    QSqlTestDatabase::Batch values;

    for (int column = 0; column < inputFile.columnCount(); column++) {
        QVariantList value;
        for (int row = 0; row < inputFile.rowCount(); row++) {
            QModelIndex index = inputFile.index(row, column);
            value << inputFile.data(index);
        }
        values.insert(column, value);
    }

    ret = d->db.execBatch(insertString, values);
    if (!ret) {
        QSqlTestLogger::fail(QString::fromLatin1("%1 (%2)").arg(testName).arg(d->db.lastError()));
        RETURN_ERROR(d->db.lastError());
    }

    const auto model = d->db.execStatement(query);

    if (model->rowCount() != outputFile.rowCount()) {
        const auto &err = QLatin1String("Expected %1 rows, got %2")
                                  .arg(outputFile.rowCount())
                                  .arg(model->rowCount());
        QSqlTestLogger::fail(QString::fromLatin1("%1 (%2)").arg(err));
        RETURN_ERROR(err);
    }
    if (model->columnCount() != outputFile.columnCount()) {
        const auto &err = QLatin1String("Expected %2 columns, got %3")
                                  .arg(outputFile.columnCount())
                                  .arg(model->columnCount());
        QSqlTestLogger::fail(QString::fromLatin1("%1 (%2)").arg(testName).arg(err));
        RETURN_ERROR(err);
    }

    const auto rowCount = model->rowCount();
    const auto columnCount = model->columnCount();
    for (int row = 0; row < rowCount; row++) {
        for (int column = 0; column < columnCount; column++) {
            const auto csvIndex = outputFile.index(row, column);
            const auto csvValue = outputFile.data(csvIndex);
            const auto sqlIndex = model->index(row, column);
            const auto sqlValue = model->data(sqlIndex);

            if (csvValue != sqlValue) {
                const auto err = QLatin1String("row %1, column %2 - expected %3, got %4")
                                         .arg(row)
                                         .arg(column)
                                         .arg(csvValue.toString())
                                         .arg(sqlValue.toString());
                QSqlTestLogger::fail(QString::fromLatin1("%1 (%2)").arg(testName).arg(err));
                return false;
            }
        }
    }

    QSqlTestLogger::pass(testName);
    RETURN_SUCCESS();
}

bool QSqlTest::start(const QString &fileName)
{
    bool ret;

    QUrl url(fileName);
    const QString &filename = url.isLocalFile() ? url.toLocalFile() : fileName;

    ret = load(filename);
    if (!ret) {
        QSqlTestLogger::print(QLatin1String("<red>Error loading %1</red>").arg(filename));
        return false;
    }

    QSqlTestLogger::start(d->json.name());

    QSqlTestLogger::print("Connecting to Database");
    ret = connect();
    if (!ret) {
        QSqlTestLogger::print("<red>FAILED!</red>");
        return false;
    }
    QSqlTestLogger::print("<green>SUCCESS!</green>");

    ret = createTables();
    if (!ret) {
        return false;
    }

    for (const auto testName : d->json.tests().keys()) {
        int index = 0;
        for (int i = 0; i < d->tests.size(); i++) {
            if (d->tests.at(i)->name() == testName) {
                index = i;
            }
        }

        const bool skip = d->json.tests().value(testName).skip;
        if (skip) {
            QSqlTestLogger::skip(testName);
            auto testcase = new QSqlTestCase(testName);
            testcase->setStatus("Skip");
            testcase->setFinished(true);
            d->tests.replace(index, testcase);
            d->skipped++;
            emit update(SqlTestUpdate::TestSkipped);
            emit testsChanged();
            continue;
        }

        ret = startTest(testName);

        auto testcase = new QSqlTestCase(testName);
        testcase->setStatus(ret ? "Pass" : "Fail");
        testcase->setFinished(true);
        testcase->setPassed(ret);
        d->tests.replace(index, testcase);
        if (ret) {
            d->passed++;
            emit update(SqlTestUpdate::TestPassed);
        } else {
            d->failed++;
            emit update(SqlTestUpdate::TestFailed);
        }
        emit testsChanged();
    }

    d->db.close();

    QSqlTestLogger::finish(d->json.name());

    return true;
}
