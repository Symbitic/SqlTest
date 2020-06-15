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

    QSqlTest test;
    bool ret;

    const auto file = parser.positionalArguments().at(0);

    ret = test.start(file);
    if (!ret) {
        return 1;
    }

    return 0;
}
