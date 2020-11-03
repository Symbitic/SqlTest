#include "QSqlTestJsonFile.h"
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QtGlobal>
#include <QDebug>

#define CHECK_REQUIREMENT(cond, err)                                                               \
    if (!cond) {                                                                                   \
        emit error(getErrorString(err));                                                           \
        return false;                                                                              \
    }

enum Errors
{
    NoError,
    Uninitialized,
    InvalidJsonFile,
    InvalidRootType,
    MissingName,
    MissingDatabase,
    MissingDatabaseDriver,
    MissingQueries,
    MissingTables,
    MissingTests,
    InvalidQuery,
    InvalidTableSchema,
};

static const char *getErrorString(Errors err)
{
    switch (err) {
        case NoError:
            return "No Error";
        case Uninitialized:
            return "File not loaded yet";
        case InvalidJsonFile:
            return "Not a valid JSON file";
        case InvalidRootType:
            return "Project file is not valid";
        case MissingName:
            return "Missing required 'name' property";
        case MissingDatabase:
            return "Missing required 'database' property";
        case MissingDatabaseDriver:
            return "Missing required 'database.driver' property";
        case MissingQueries:
            return "Missing required 'queries' property";
        case MissingTables:
            return "Missing required 'tables' property";
        case MissingTests:
            return "Missing required 'tests' property";
        case InvalidQuery:
            return "Query %1 is not a string";
        case InvalidTableSchema:
            return "Table schema '%1' is invalid";
        default:
            return "Unrecognized error";
    }
}

struct QSqlTestJsonFilePrivate
{
    QSqlTestJsonFilePrivate()
        : name()
        , driver()
        , database()
        , username()
        , password()
        , hostname()
        , queries()
        , tables()
        , tests()
    {
    }

    QString name;
    QString driver;
    QString database;
    QString username;
    QString password;
    QString hostname;
    int port;
    QStringHash queries;
    QSqlTestJsonTables tables;
    QSqlTestJsonTests tests;
};

QSqlTestJsonFile::QSqlTestJsonFile(QObject *parent)
    : QObject(parent)
    , d(new QSqlTestJsonFilePrivate)
{
}

const QString QSqlTestJsonFile::name() const
{
    return d->name;
}

const QString QSqlTestJsonFile::driver() const
{
    return d->driver;
}

const QString QSqlTestJsonFile::database() const
{
    return d->database;
}

const QString QSqlTestJsonFile::username() const
{
    return d->username;
}

const QString QSqlTestJsonFile::password() const
{
    return d->password;
}

const QString QSqlTestJsonFile::hostname() const
{
    return d->hostname;
}

int QSqlTestJsonFile::port() const
{
    return d->port;
}

QStringHash QSqlTestJsonFile::queries() const
{
    return d->queries;
}

QSqlTestJsonTables QSqlTestJsonFile::tables() const
{
    return d->tables;
}

QSqlTestJsonTests QSqlTestJsonFile::tests() const
{
    return d->tests;
}

bool QSqlTestJsonFile::load(const QString &filename)
{
    if (!QFile::exists(filename)) {
        emit error(tr("%1 does not exist").arg(filename));
        return false;
    }

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        emit error(tr("Error opening %1").arg(filename));
        return false;
    }

    QByteArray data = file.readAll();
    QJsonParseError parseError;
    QJsonDocument doc;
    doc = QJsonDocument::fromJson(data, &parseError);
    if (parseError.error != QJsonParseError::ParseError::NoError) {
        // TODO: Need error location reporting.
        emit error(parseError.errorString());
        return false;
    } else if (!doc.isObject()) {
        emit error(getErrorString(Errors::InvalidRootType));
        return false;
    }

    const QJsonObject rootObject = doc.object();

    CHECK_REQUIREMENT(!rootObject.value("name").toString().isEmpty(), Errors::MissingName);
    CHECK_REQUIREMENT(rootObject.value("database").isObject(), Errors::MissingDatabase);
    CHECK_REQUIREMENT(!rootObject.value("database").toObject().value("driver").toString().isEmpty(),
                      Errors::MissingDatabaseDriver);
    CHECK_REQUIREMENT(rootObject.value("queries").isObject(), Errors::MissingQueries);
    CHECK_REQUIREMENT(!rootObject.value("queries").toObject().isEmpty(), Errors::MissingQueries);
    CHECK_REQUIREMENT(rootObject.value("tables").isObject(), Errors::MissingTables);
    CHECK_REQUIREMENT(!rootObject.value("tables").toObject().isEmpty(), Errors::MissingTables);
    CHECK_REQUIREMENT(rootObject.value("tests").isObject(), Errors::MissingTests);
    CHECK_REQUIREMENT(!rootObject.value("tests").toObject().isEmpty(), Errors::MissingTests);

    d->name = rootObject.value("name").toString();

    const auto dbObj = rootObject.value("database").toObject();
    bool converted;

    d->driver = dbObj.value("driver").toString();
    d->database = dbObj.value("database").toString();
    d->username = dbObj.value("username").toString();
    d->password = dbObj.value("password").toString();
    d->hostname = dbObj.value("hostname").toString();
    d->port = dbObj.value("database").toString().toInt(&converted);

    // TODO: use const list of supported drivers.
    if (d->driver == "sqlite") {
    } else {
        emit error("Unrecognized driver");
        return false;
    }

    if (!converted) {
        d->port = -1;
    }

    const auto queries = rootObject.value("queries").toObject();

    for (const auto query : queries.keys()) {
        const auto queryString = queries.value(query).toString();
        if (query.isEmpty()) {
            const auto err = tr(getErrorString(Errors::InvalidQuery)).arg(query);
            emit error(err);
            return false;
        }
        d->queries.insert(query, queryString);
    }

    const auto tables = rootObject.value("tables").toObject();
    for (const auto tableName : tables.keys()) {
        const auto tableArray = tables.value(tableName).toArray();

        if (tableArray.size() == 0) {
            const auto err = tr(getErrorString(Errors::InvalidTableSchema)).arg(tableName);
            emit error(err);
            return false;
        }

        QStringList list;
        for (const auto str : tableArray) {
            list << str.toString();
        }
        d->tables.insert(tableName, list);
    }

    const auto tests = rootObject.value("tests").toObject();
    for (const auto testName : tests.keys()) {
        // TODO: Verify all of these.
        QSqlTestJsonCase test;
        if (!tests.value(testName).isObject() || tests.value(testName).toObject().size() == 0) {
            //
        }
        const auto testObject = tests.value(testName).toObject();
        const auto results = testObject.value("results").toString();
        const auto query = testObject.value("query").toString();
        const auto tablesObject = testObject.value("tables").toObject();
        const auto varsObject = testObject.value("variables").toObject();
        const auto fail = testObject.value("fail").toBool();
        const auto skip = testObject.value("skip").toBool();
        test.results = results;
        test.queryName = query;
        test.fail = fail;
        test.skip = skip;

        for (const auto key : tablesObject.keys()) {
            const auto val = tablesObject.value(key).toString();
            test.tableNames.insert(key, val);
        }
        for (const auto key : varsObject.keys()) {
            const auto val = varsObject.value(key).toString();
            test.varNames.insert(key, val);
        }
        d->tests.insert(testName, test);
    }

    return true;
}
