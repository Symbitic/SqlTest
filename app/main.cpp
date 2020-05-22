#include "database.h"
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

    // QQuickStyle::setStyle(QStringLiteral("Material"));
    // QQuickStyle::setStyle(QStringLiteral("Universal"));
    // static QStringList availableStyles()

    qmlRegisterType<Database>("SqlTester", 1, 0, "Database");

    QQmlApplicationEngine engine;

    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return app.exec();
}
