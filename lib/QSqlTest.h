/**
 * @file QSqlTest.h
 * @brief Load and execute a QSqlTest project.
 */

#ifndef QSQLTEST_H
#define QSQLTEST_H

#include <QObject>
#include <QString>

QT_FORWARD_DECLARE_CLASS(QSqlTestPrivate)

class QSqlTest : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool loaded READ loaded)
    Q_PROPERTY(bool connected READ connected)

public:
    explicit QSqlTest(QObject *parent = nullptr);

    bool connected() const;
    bool loaded() const;

    bool load(const QString &jsonFilename);
    bool connect();
    bool createTables();
    bool startTest(const QString &testName);

    static void setLogging(bool enable);

    /**
     * Load and execute tests.
     */
    bool start(const QString &fileName);

signals:
    void error(const QString &err);

private:
    QSqlTestPrivate *d;
};

#endif
