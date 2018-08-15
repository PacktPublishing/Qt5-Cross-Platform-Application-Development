#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QRect>
#include <QDebug>

#include "gameinterface.h"
#include "player.h"


int main(int argc, char *argv[])
{
    // Enable High Dpi Scaling because ....
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    // Create our application, which controls our event loop
    QGuiApplication app(argc, argv);

    qmlRegisterType<GameInterface>("GameInterfaces", 1, 0, "GameInterface");

    // Create our QML application engine, which handles our QML
    QQmlApplicationEngine engine;
    // Load our `main.qml` page
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    // Check to see if we loaded the file correctly
    if (engine.rootObjects().isEmpty())
        // if we didn't load correctly, exit the main loop with the error/integer, `-1`
        return -1;

    QObject *window = engine.rootObjects().first();
    GameInterface *game_interface = window->findChild<GameInterface *>("game_interface");
    qDebug() << game_interface << window << window->children();
    // game_interface->set_game_size(1000, 1000);

    Player *this_player = new Player("DEFAULT_AUTH", new QRect(0, 1000, 0, 1000), game_interface);
    return app.exec();
}
