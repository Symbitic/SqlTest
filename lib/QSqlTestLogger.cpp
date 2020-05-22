#include "QSqlTestLogger.h"
#include <QDebug>
#include <QMap>
#include <QString>
#include <QRegExp>
#include <cstdio>
#include <iostream>
#include <map>

#if _WIN32
#    include <io.h>
#    define ISATTY _isatty
#    define FILENO _fileno
#else
#    include <unistd.h>
#    define ISATTY isatty
#    define FILENO fileno
#endif

#define ESCAPE "\033"
#define RESET "\033[0m"

struct QSqlTestLoggerPrivate
{
    enum Colors
    {
        Red,
        Green,
        Blue,
        Yellow,
        Magenta,
        Cyan,

        BoldRed,
    };

    QSqlTestLoggerPrivate()
        : m_passed(0)
        , m_failed(0)
        , m_colors()
        , m_enabled(true)
    {
        m_hascolor = ISATTY(FILENO(stdout));
        m_colors[Red] = "[1;31m";
        m_colors[Green] = "[1;32m";
        m_colors[Blue] = "[1;34m";
        m_colors[Magenta] = "[1;95m";
        m_colors[Yellow] = "[1;33m";
        m_colors[Cyan] = "[1;96m";
    };

    QString format(QString str)
    {
        return str
                .replace(QLatin1String("<red>"), ESCAPE + QLatin1String(m_colors[Red]),
                         Qt::CaseInsensitive)
                .replace(QLatin1String("<green>"), ESCAPE + QLatin1String(m_colors[Green]),
                         Qt::CaseInsensitive)
                .replace(QLatin1String("<blue>"), ESCAPE + QLatin1String(m_colors[Blue]),
                         Qt::CaseInsensitive)
                .replace(QLatin1String("<yellow>"), ESCAPE + QLatin1String(m_colors[Yellow]),
                         Qt::CaseInsensitive)
                .replace(QLatin1String("<magenta>"), ESCAPE + QLatin1String(m_colors[Magenta]),
                         Qt::CaseInsensitive)
                .replace(QLatin1String("<cyan>"), ESCAPE + QLatin1String(m_colors[Cyan]),
                         Qt::CaseInsensitive)
                .replace(QRegExp("</[^>]*>"), QLatin1String(RESET));
    };

    typedef QMap<Colors, const char *> ColorNames;

    ColorNames m_colors;
    bool m_hascolor;
    bool m_enabled;
    int m_passed;
    int m_failed;
};

static QSqlTestLoggerPrivate *logger = new QSqlTestLoggerPrivate();

QSqlTestLogger::QSqlTestLogger()
    : d(new QSqlTestLoggerPrivate)
{
    logger = d;
}

void QSqlTestLogger::setLogging(bool enable)
{
    logger->m_enabled = enable;
}

void QSqlTestLogger::start(const QString &name)
{
    const QString &msg = QStringLiteral("********* Starting <blue>%1</blue> *********").arg(name);
    print(msg);
}

void QSqlTestLogger::finish(const QString &name)
{
    const QString &msg1 = QStringLiteral("Results: %1 passed, %2 failed")
                                  .arg(logger->m_passed)
                                  .arg(logger->m_failed);
    const QString &msg2 = QStringLiteral("********* Finished <blue>%1</blue> *********").arg(name);
    print(msg1);
    print(msg2);
}

void QSqlTestLogger::pass(const QString &test)
{
    const QString &msg = QStringLiteral("<green>PASS</green>   : %1").arg(test);
    print(msg);
    logger->m_passed++;
}

void QSqlTestLogger::fail(const QString &test)
{
    const QString &msg = QStringLiteral("<red>FAIL</red>   : %1").arg(test);
    print(msg);
    logger->m_failed++;
}

void QSqlTestLogger::skip(const QString &test)
{
    const QString &msg = QStringLiteral("<cyan>SKIP</cyan>   : %1").arg(test);
    print(msg);
}

void QSqlTestLogger::print(QString msg)
{
    if (!logger->m_enabled) {
        return;
    }
    QString tmp = logger->format(msg);
    qInfo() << qPrintable(tmp);
}
