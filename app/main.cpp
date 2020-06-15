#include "QSqlTest.h"
#include <QApplication>
#include <QDebug>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QIcon>

Q_DECL_EXPORT
int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    QApplication app(argc, argv);
    app.setApplicationDisplayName("SQL Tester");
    app.setOrganizationName("Alex Shaw");
    app.setWindowIcon(QPixmap(":/data/qt.png"));

    qmlRegisterType<QSqlTest>("SqlTester", 1, 0, "SqlTest");

    QQmlApplicationEngine engine;

    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return app.exec();
}
