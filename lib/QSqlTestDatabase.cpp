#include "QSqlTestDatabase.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlRecord>

#include <QDebug>

struct QSqlTestDatabasePrivate
{
    QSqlTestDatabasePrivate()
        : port(-1)
        , connected(false)
        , lastError()
    {
    }

    int port;
    bool connected;
    QString lastError;
};

QSqlTestDatabase::QSqlTestDatabase(QObject *parent)
    : QObject(parent)
    , d(new QSqlTestDatabasePrivate)
{
}

void QSqlTestDatabase::close()
{
    if (connected()) {
        QSqlDatabase::removeDatabase("QSqlTestData");
        d->connected = false;
    }
}

bool QSqlTestDatabase::connected() const
{
    return d->connected;
}

QString QSqlTestDatabase::lastError() const
{
    return d->lastError;
}

bool QSqlTestDatabase::connect(const QSqlTestDatabaseSettings &settings)
{
    bool usedefaultdatabase = true;

    if (usedefaultdatabase) {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "QSqlTestData");
        db.setDatabaseName(":memory:");
        if (!db.open()) {
            d->lastError = db.lastError().text();
            return false;
        }
    } else {
        QSqlDatabase db = QSqlDatabase::addDatabase(settings.driver, "QSqlTestData");
        db.setDatabaseName(settings.database);
        db.setHostName(settings.hostname);
        db.setPort(settings.port);
        if (!db.open(settings.username, settings.password)) {
            d->lastError = db.lastError().text();
            QSqlDatabase::removeDatabase("QSqlTestData");
            return false;
        }
    }

    d->connected = true;
    d->lastError = "";

    return true;
}

bool QSqlTestDatabase::exec(const QString &q)
{
    QSqlDatabase db = QSqlDatabase::database("QSqlTestData");
    QSqlQuery query(db);
    if (!query.exec(q)) {
        d->lastError = query.lastError().text();
        return false;
    }

    return true;
}

bool QSqlTestDatabase::execBatch(const QString &queryString, Batch batchValues)
{
    bool ret;
    QSqlDatabase db = QSqlDatabase::database("QSqlTestData");
    QSqlQuery q("", db);

    ret = q.prepare(queryString);
    if (!ret) {
        d->lastError = q.lastError().text();
        return false;
    }

    for (int i = 0; i < batchValues.size(); i++) {
        const QVariantList values = batchValues.value(i);
        q.addBindValue(values);
    }

    ret = q.execBatch();
    if (!ret) {
        d->lastError = q.lastError().text();
        return false;
    }

    return ret;
}

QSqlQueryModel *QSqlTestDatabase::execStatement(const QString &queryString)
{
    QSqlDatabase db = QSqlDatabase::database("QSqlTestData");
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery(queryString, db);

    if (model->lastError().isValid()) {
        d->lastError = model->lastError().text();
        return nullptr;
    }

    return model;
}
