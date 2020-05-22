#include "QSqlTest.h"
#include "QSqlTestCsvFile.h"
#include "QSqlTestJsonFile.h"
#include "QSqlTestDatabase.h"
#include "QSqlTestLogger.h"
#include <QFileInfo>
#include <QDir>
#include <QRegExp>

#include <QDebug>

struct QSqlTestPrivate
{
    QSqlTestPrivate()
        : loaded(false)
        , db()
        , json()
    {
    }

    bool loaded;
    QSqlTestDatabase db;
    QSqlTestJsonFile json;
    QString dirName;
};

QSqlTest::QSqlTest(QObject *parent)
    : QObject(parent)
    , d(new QSqlTestPrivate)
{
    /* clang-format off */
    const auto errorHandler = [=](const QString &err) {
        emit error(err);
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

void QSqlTest::setLogging(bool enable)
{
    QSqlTestLogger::setLogging(enable);
}

bool QSqlTest::load(const QString &jsonFilename)
{
    bool ret = d->json.load(jsonFilename);
    if (!ret) {
        return false;
    }

    d->dirName = QFileInfo(jsonFilename).absolutePath();

    d->loaded = true;
    return true;
}

bool QSqlTest::connect()
{
    QSqlTestLogger::print("Connecting to Database");
    if (!loaded()) {
        QSqlTestLogger::print("<red>FAILED!</red>");
        return false;
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
        return false;
    }

    QSqlTestLogger::print("<green>SUCCESS!</green>");
    return true;
}

bool QSqlTest::createTables()
{
    QSqlTestLogger::print("Creating test tables");
    if (!connected()) {
        QSqlTestLogger::print("<red>FAILED!</red>");
        return false;
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
            return false;
        }
    }

    QSqlTestLogger::print("<green>SUCCESS!</green>");
    return true;
}

bool QSqlTest::startTest(const QString &testName)
{
    QSqlTestCsvFile2 inputFile;
    QSqlTestCsvFile2 outputFile;
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
        QSqlTestLogger::fail(QString::fromLatin1("%1 (error loading input file %2)")
                                     .arg(testName)
                                     .arg(inputFilename));
        return false;
    }

    ret = outputFile.load(outputFilename);
    if (!ret) {
        QSqlTestLogger::fail(QString::fromLatin1("%1 (error loading output file %2)")
                                     .arg(testName)
                                     .arg(outputFilename));
        return false;
    }

    QString columnNames = inputFile.headerData(0).toString();
    QString inserts = "?";
    for (int i = 1; i < inputFile.columnCount(); i++) {
        columnNames.append(", " + inputFile.headerData(i).toString());
        inserts.append(", ?");
    }

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
    }

    const auto model = d->db.execStatement(query);

    if (model->rowCount() != outputFile.rowCount()) {
        QSqlTestLogger::fail(QString::fromLatin1("%1 (expected %2 rows, Got %3)")
                                     .arg(testName)
                                     .arg(outputFile.rowCount())
                                     .arg(model->rowCount()));
        return false;
    }
    if (model->columnCount() != outputFile.columnCount()) {
        QSqlTestLogger::fail(QString::fromLatin1("%1 (expected %2 columns, Got %3)")
                                     .arg(testName)
                                     .arg(outputFile.columnCount())
                                     .arg(model->columnCount()));
        return false;
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
                const auto err = QString::fromLatin1("%1 (row %2, column %3 - expected %4, got %5)")
                                         .arg(testName)
                                         .arg(row)
                                         .arg(column)
                                         .arg(csvValue.toString())
                                         .arg(sqlValue.toString());
                QSqlTestLogger::fail(err);
                return false;
            }
        }
    }

    QSqlTestLogger::pass(testName);

    return true;
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
