#ifndef QSQLTESTLOGGER_H
#define QSQLTESTLOGGER_H

#include <QString>

QT_FORWARD_DECLARE_CLASS(QSqlTestLoggerPrivate)

class QSqlTestLogger
{
public:
    QSqlTestLogger();
    ~QSqlTestLogger();

    static void start(const QString &name);
    static void finish(const QString &name);
    static void pass(const QString &test);
    static void fail(const QString &test);
    static void skip(const QString &test);
    static void print(QString msg);

    static void setLogging(bool enable);

private:
    QSqlTestLoggerPrivate *d;
};

#endif
