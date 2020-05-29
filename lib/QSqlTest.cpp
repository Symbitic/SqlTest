#include "QSqlTest.h"
#include "QSqlTestCsvFile.h"
#include "QSqlTestJsonFile.h"
#include "QSqlTestDatabase.h"
#include "QSqlTestLogger.h"
#include <QFileInfo>
#include <QDir>
#include <QRegExp>

#include <QDebug>

#define RETURN_ERROR(msg)                                                                          \
    d->error = msg;                                                                                \
    emit errorMsg(msg);                                                                            \
    return false;

#define RETURN_SUCCESS()                                                                           \
    if (d->error != "No Error") {                                                                  \
        d->error = "No Error";                                                                     \
        emit errorMsg("No Error");                                                                 \
    }                                                                                              \
    return true;

struct QSqlTestPrivate
{
    QSqlTestPrivate()
        : loaded(false)
        , db()
        , json()
        , error("No Error")
        , succeeded(0)
        , failed(0)
    {
    }

    bool loaded;
    QSqlTestDatabase db;
    QSqlTestJsonFile json;
    QString dirName;
    QString error;
    int succeeded;
    int failed;
};

QSqlTest::QSqlTest(QObject *parent)
    : QObject(parent)
    , d(new QSqlTestPrivate)
{
    /* clang-format off */
    const auto errorHandler = [=](const QString &err) {
        d->error = err;
        emit errorMsg(err);
    };
    QObject::connect(&d->db, &QSqlTestDatabase::error, errorHandler);
    QObject::connect(&d->json, &QSqlTestJsonFile::error, errorHandler);
    /* clang-format on */
}

bool QSqlTest::connected() const
{
    return d->db.connected();
}

bool QSqlTest::loaded() const
{
    return d->loaded;
}

QString QSqlTest::error() const
{
    return d->error;
}

int QSqlTest::succeeded() const
{
    return d->succeeded;
}

int QSqlTest::failed() const
{
    return d->failed;
}

void QSqlTest::setLogging(bool enable)
{
    QSqlTestLogger::setLogging(enable);
}

bool QSqlTest::load(const QString &jsonFilename)
{
    bool ret = d->json.load(jsonFilename);
    if (!ret) {
        RETURN_ERROR("Error loading file");
    }

    d->dirName = QFileInfo(jsonFilename).absolutePath();

    d->loaded = true;
    RETURN_SUCCESS();
}

bool QSqlTest::connect()
{
    QSqlTestLogger::print("Connecting to Database");
    if (!loaded()) {
        QSqlTestLogger::print("<red>FAILED!</red>");
        RETURN_ERROR("Not connected");
    }

    QSqlTestDatabaseSettings settings;
    settings.driver = d->json.driver();
    settings.database = d->json.database();
    settings.username = d->json.username();
    settings.password = d->json.password();
    settings.hostname = d->json.hostname();
    settings.port = d->json.port();

    bool ret = d->db.connect(settings);
    if (!ret) {
        QSqlTestLogger::print("<red>FAILED!</red>");
        RETURN_ERROR(d->db.lastError());
    }

    QSqlTestLogger::print("<green>SUCCESS!</green>");
    RETURN_SUCCESS();
}

bool QSqlTest::createTables()
{
    QSqlTestLogger::print("Creating test tables");
    if (!connected()) {
        QSqlTestLogger::print("<red>FAILED!</red>");
        RETURN_ERROR("Not connected");
    }

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
            RETURN_ERROR(d->db.lastError());
        }
    }

    QSqlTestLogger::print("<green>SUCCESS!</green>");
    RETURN_SUCCESS();
}

bool QSqlTest::startTest(const QString &testName)
{
    QSqlTestCsvFile inputFile;
    QSqlTestCsvFile outputFile;
    const auto testCase = d->json.tests().value(testName);
    const auto tableName = testCase.tableName;
    auto inputFilename = testCase.input;
    auto outputFilename = testCase.output;
    const auto queryName = testCase.queryName;
    const auto tableNames = testCase.tableNames;
    auto query = d->json.queries().value(queryName);
    bool ret;

    for (const auto tableName : tableNames.keys()) {
        const auto before = QString("{{%1}}").arg(tableName);
        const auto after = tableNames.value(tableName);
        query.replace(before, after);
    }

    inputFilename = QDir(d->dirName).filePath(inputFilename);
    outputFilename = QDir(d->dirName).filePath(outputFilename);

    ret = inputFile.load(inputFilename);
    if (!ret) {
        const auto &err = QLatin1String("Error loading input file %1").arg(inputFilename);
        QSqlTestLogger::fail(QString::fromLatin1("%1 (%2)").arg(testName).arg(err));
        RETURN_ERROR(err);
    }

    ret = outputFile.load(outputFilename);
    if (!ret) {
        const auto &err = QLatin1String("Error loading output file %1").arg(outputFilename);
        QSqlTestLogger::fail(QString::fromLatin1("%1 (%2)").arg(testName).arg(err));
        RETURN_ERROR(err);
    }

    QString columnNames = inputFile.headerData(0).toString();
    QString inserts = "?";
    for (int i = 1; i < inputFile.columnCount(); i++) {
        columnNames.append(", " + inputFile.headerData(i).toString());
        inserts.append(", ?");
    }

    // Do not check return value.
    ret = d->db.exec(QString::fromLatin1("DELETE FROM %1").arg(tableName));

    const auto insertString = QString("INSERT INTO %1 (%2) VALUES (%3)")
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

    ret = load(fileName);
    if (!ret) {
        QSqlTestLogger::print(QString::fromLatin1("<red>Error loading %1</red>").arg(fileName));
        return false;
    }

    QSqlTestLogger::start(d->json.name());

    ret = connect();
    if (!ret) {
        return false;
    }

    ret = createTables();
    if (!ret) {
        return false;
    }

    for (const auto testName : d->json.tests().keys()) {
        const bool skip = d->json.tests().value(testName).skip;
        if (skip) {
            QSqlTestLogger::skip(testName);
            continue;
        }

        ret = startTest(testName);
        if (!ret) {
            // return false;
        }
    }

    d->db.close();

    QSqlTestLogger::finish(d->json.name());

    return true;
}
