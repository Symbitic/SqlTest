#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlQueryModel>

class DatabasePrivate;
QT_FORWARD_DECLARE_CLASS(QStringList)

class Database : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QSqlQueryModel* model READ model NOTIFY modelChanged)
    Q_PROPERTY(QStringList drivers READ drivers WRITE setDrivers NOTIFY driversChanged)
    Q_PROPERTY(bool connected READ connected WRITE setConnected NOTIFY connectedChanged)

public:
    explicit Database(QObject *parent = nullptr);

    QSqlQueryModel* model() const;
    QStringList drivers() const;
    bool connected() const;

    void setDrivers(QStringList& list);
    void setConnected(bool connected);

signals:
    void modelChanged();
    void driversChanged();
    void connectedChanged();
    void error(const QString &err);
    void statusMessage(const QString &msg);

public slots:
    void exec(const QString& text);
    void connect(
        const QString &driver,
        const QString &database,
        const QString &username, 
        const QString &password,
        const QString &hostname,
        const int port = -1,
        const bool usedefaultdatabase = false
    );

private:
    DatabasePrivate* d;
};

#endif
