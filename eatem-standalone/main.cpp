#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "gameinterface.h"
#include "player.h"


int main(int argc, char *argv[])
{
    // Enable High Dpi Scaling because ....
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    // Create our application, which controls our event loop
    QGuiApplication app(argc, argv);

    GameInterface game_interface;
    game_interface.set_game_size(1000, 1000);
    game_interface.start_game();

    Player *player = new Player("AUTH", game_interface.game_size(), &game_interface);
    game_interface.add_player(player);

    // Create our QML application engine, which handles our QML
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("game_interface", &game_interface);
    // Load our `main.qml` page
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));



    // Check to see if we loaded the file correctly
    if (engine.rootObjects().isEmpty())
        // if we didn't load correctly, exit the main loop with the error/integer, `-1`
        return -1;

    return app.exec();
}
