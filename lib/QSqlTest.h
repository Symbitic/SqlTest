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
    Q_PROPERTY(int succeeded READ succeeded)
    Q_PROPERTY(int failed READ failed)
    Q_PROPERTY(QString error READ error NOTIFY errorMsg)

public:
    explicit QSqlTest(QObject *parent = nullptr);

    bool connected() const;
    bool loaded() const;
    int succeeded() const;
    int failed() const;
    QString error() const;

    static void setLogging(bool enable);

    bool load(const QString &jsonFilename);
    bool connect();
    bool createTables();
    bool startTest(const QString &testName);

    /**
     * Load and execute tests.
     * @param[in] fileName JSON project file to load.
     * @returns Test results. `true` if the project was loaded and all tests executed successfully.
     * `false` otherwise.
     */
    bool start(const QString &fileName);

signals:
    void errorMsg(const QString &err);

private:
    QSqlTestPrivate *d;
};

#endif
