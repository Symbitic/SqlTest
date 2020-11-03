/**
 * @file QSqlTestJsonFile.h
 * @brief Project file for running SQL tests.
 */

#ifndef QSQLTESTJSONFILE_H
#define QSQLTESTJSONFILE_H

#include <QObject>
#include <QJsonDocument>
#include <QString>
#include <QHash>

QT_FORWARD_DECLARE_CLASS(QSqlTestJsonFilePrivate)

typedef QHash<QString, QString> QStringHash;

/**
 * A single test case.
 * The "tests" property in the JSON file.
 */
struct QSqlTestJsonCase
{
    QString results;
    QString queryName;
    bool fail;
    bool skip;
    QStringHash tableNames;
    QStringHash varNames;
};

typedef QHash<QString, QStringList> QSqlTestJsonTables;
typedef QHash<QString, QSqlTestJsonCase> QSqlTestJsonTests;

class QSqlTestJsonFile : public QObject
{
    Q_OBJECT
    Q_PROPERTY(const QString name READ name)
    Q_PROPERTY(const QString driver READ driver)
    Q_PROPERTY(const QString database READ database)
    Q_PROPERTY(const QString username READ username)
    Q_PROPERTY(const QString password READ password)
    Q_PROPERTY(const QString hostname READ hostname)
    Q_PROPERTY(int port READ port)
    Q_PROPERTY(QStringHash queries READ queries)
    Q_PROPERTY(QSqlTestJsonTables tables READ tables)
    Q_PROPERTY(QSqlTestJsonTests tests READ tests)

public:
    explicit QSqlTestJsonFile(QObject *parent = nullptr);

    const QString name() const;
    const QString driver() const;
    const QString database() const;
    const QString username() const;
    const QString password() const;
    const QString hostname() const;
    int port() const;
    QStringHash queries() const;
    QSqlTestJsonTables tables() const;
    QSqlTestJsonTests tests() const;

signals:
    void error(const QString &err);

public slots:
    bool load(const QString &filename);

private:
    QSqlTestJsonFilePrivate *d;
};

#endif
