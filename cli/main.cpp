#include "QSqlTestCsvFile.h"
#include "QSqlTestJsonFile.h"
#include "QSqlTest.h"
#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>

#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setOrganizationName("QtProject");
    QCoreApplication::setApplicationName("sqltestrunner");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);
    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::applicationName());
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("file", "The JSON configuration file.");
    parser.process(app);

    if (parser.positionalArguments().isEmpty()) {
        fputs(qPrintable(parser.helpText()), stderr);
        std::exit(EXIT_FAILURE);
    }

    const auto errorHandler = [=](const QString &err) {
        // TODO: should exit?
        qFatal("Error: %s", qPrintable(err));
    };
    QSqlTest test;
    bool ret;

    QObject::connect(&test, &QSqlTest::error, errorHandler);

    const auto file = parser.positionalArguments().at(0);

    ret = test.start(file);
    if (!ret) {
        return 1;
    }

    return 0;
}
