#include "database.h"
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QStringList>
#include <QUrl>

struct DatabasePrivate {
    DatabasePrivate()
    : url("")
    , db()
    , model(new QSqlQueryModel)
    , drivers()
    , connected(false)
    {
        QStringList drivers = QSqlDatabase::drivers();

        // remove compat names
        drivers.removeAll("QMYSQL3");
        drivers.removeAll("QOCI8");
        drivers.removeAll("QODBC3");
        drivers.removeAll("QPSQL7");
        drivers.removeAll("QTDS7");

        this->drivers = drivers;
    }

    QUrl url;
    QSqlDatabase db;
    QSqlQueryModel* model;
    QStringList drivers;
    bool connected;
};

Database::Database(QObject *parent)
    : QObject(parent)
    , d(new DatabasePrivate)
{
}

void Database::connect(const QString &driver, const QString &database,
                       const QString &username,  const QString &password,
                       const QString &hostname, const int port,
                       const bool usedefaultdatabase)
{
    /*
    qDebug() << "Connect";
    qDebug() << "Driver: " << driver;
    qDebug() << "Database: " << database;
    qDebug() << "Username: " << username;
    qDebug() << "Password: " << password;
    qDebug() << "Hostname: " << hostname;
    qDebug() << "Port: " << port;
    qDebug() << "UseDefaultDatabase: " << usedefaultdatabase;
    qDebug() << "-----";
    */
    if (usedefaultdatabase) {
        QSqlDatabase::database("in_mem_db", false).close();
        QSqlDatabase::removeDatabase("in_mem_db");
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "in_mem_db");
        db.setDatabaseName(":memory:");
        if (!db.open()) {
            emit error(db.lastError().text());
            qDebug() << db.lastError().text();
            return;
        }
        d->db = db;
        QSqlQuery q("", db);
        q.exec("drop table Names");
        q.exec("create table Names (id integer primary key, Firstname varchar, Lastname varchar, City varchar)");
        q.exec("insert into Names values (0, 'Sala', 'Palmer', 'Morristown')");
        q.exec("insert into Names values (1, 'Christopher', 'Walker', 'Morristown')");
        q.exec("insert into Names values (2, 'Donald', 'Duck', 'Andeby')");
        q.exec("insert into Names values (3, 'Buck', 'Rogers', 'Paris')");
        q.exec("insert into Names values (4, 'Sherlock', 'Holmes', 'London')");
    } else {
        qDebug() << "TODO";
        QSqlDatabase db = QSqlDatabase::addDatabase(driver, "AvengerData");
        db.setDatabaseName(database);
        db.setHostName(hostname);
        db.setPort(port);
        if (!db.open(username, password)) {
            emit error(tr("Unable to open database - %1").arg(db.lastError().text()));
            qDebug() << db.lastError().text();
            QSqlDatabase::removeDatabase("AvengerData");
            return;
        }
    }
    setConnected(true);
}

void Database::exec(const QString& text)
{
    QSqlQuery query(d->db);
    if (!query.exec("SELECT * FROM Names")) {
        qFatal("SELECT query failed: %s", qPrintable(query.lastError().text()));
        emit statusMessage(tr("Query failed: %1").arg(query.lastError().text()));
        return;
    }

    d->model->setQuery(query);
    if (d->model->lastError().type() != QSqlError::NoError) {
        qFatal("Cannot set query: %s", qPrintable(d->model->lastError().text()));
        emit statusMessage(d->model->lastError().text());
        return;
    }
    d->model->setHeaderData(0, Qt::Horizontal, tr("ID"));
    d->model->setHeaderData(1, Qt::Horizontal, tr("First Name"));
    d->model->setHeaderData(3, Qt::Horizontal, tr("Last Name"));
    d->model->setHeaderData(4, Qt::Horizontal, tr("City"));
    emit modelChanged();

    if (d->model->query().isSelect()) {
        emit statusMessage(tr("Query OK."));
    } else {
        emit statusMessage(tr("Query OK (%1 rows affected)").arg(d->model->query().numRowsAffected()));
    }
}

QSqlQueryModel* Database::model() const
{
    return d->model;
}

QStringList Database::drivers() const
{
    return d->drivers;
}

bool Database::connected() const
{
    return d->connected;
}

void Database::setDrivers(QStringList& drivers)
{
    d->drivers = drivers;
    emit driversChanged();
}

void Database::setConnected(bool connected)
{
    d->connected = connected;
    emit connectedChanged();
}
