/**
 * @file QSqlTestDatabase.h
 * @brief Connection to a SQL database for testing.
 */

#ifndef QSQLTESTDATABASE_H
#define QSQLTESTDATABASE_H

#include <QObject>
#include <QString>
#include <QVariantList>
#include <QSqlQueryModel>

QT_FORWARD_DECLARE_CLASS(QSqlTestDatabasePrivate)

struct QSqlTestDatabaseSettings
{
    QString driver;
    QString database;
    QString username;
    QString password;
    QString hostname;
    int port = -1;
};

class QSqlTestDatabase : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool connected READ connected)
    Q_PROPERTY(QString lastError READ lastError)

public:
    explicit QSqlTestDatabase(QObject *parent = nullptr);

    bool connected() const;
    QString lastError() const;

    using Batch = QMap<int, QVariantList>;

    bool exec(const QString &queryString);
    bool execBatch(const QString &queryString, Batch batchValues);
    QSqlQueryModel *execStatement(const QString &queryString);

    void close();

signals:
    void error(const QString &err);

public slots:
    bool connect(const QSqlTestDatabaseSettings &settings);

private:
    QSqlTestDatabasePrivate *d;
};

#endif
