#include <QCoreApplication>
#include <QWebSocketServer>

#include <QTimer>

#include <QObject>
#include <QVariantList>

#include "gameinterface.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QWebSocketServer *server = new QWebSocketServer("eatme", QWebSocketServer::SecureMode);
    bool listening = server->listen(80);

    Q_UNUSED(listening)

    // http://doc.qt.io/qt-5/qtwebchannel-chatclient-qml-qmlchatclient-qml.html
    QObject::connect(server, &QWebSocketServer::newConnection, [](){
        QWebSocket *client = server->nextPendingConnection();
    });

    // Get the root context from our QML engine
    QQmlContext *root_context = engine.rootContext();


    // ------------------------------------------
    // FIXME: This code is not in the right place
    // ------------------------------------------


    // Step 1: get access to the root object
    // NOTE: the root object is currently the ApplicationWindow
    QObject *application_object = engine.rootObjects().first();
    QObject *window_object = application_object->property("window").value<QObject*>();

    // ------------------------------------------

    // Create our `GameInterface`, an interface which abstracts some of the game creation
    // aspects for us,  including:
    // 1. creating food
    // 2. creating viruses
    // 3. Creating a list of players
    GameInterface game_interface;

    // We'll get out everything from the `game_interface` creates for us, and track
    // it in the QML context using the `setContextProperty` method.
    // The first argument in the method call is the name/accessor we'll use in QML
    root_context->setContextProperty("viruses", game_interface.get_viruses());
    root_context->setContextProperty("feed", game_interface.get_food());
    root_context->setContextProperty("players", game_interface.get_players());

    // NOTE: this method call  will go away when we set up the logic to create a player on arrival
    root_context->setContextProperty("this_player", game_interface.get_this_player());

    qDebug() << window_object;
    QObject::connect(application_object, SIGNAL(height(int)), &game_interface, SLOT(set_game_height(int)));
    QObject::connect(application_object, SIGNAL(width(int)), &game_interface, SLOT(set_game_width(int)));

    // We need to increment the game in a set amount of time.
    // We use a `QTimer` called `game_timestep` to trigger the game incremention
    QTimer game_timestep;
    // the interval for our time step is 100 milliseconds
    game_timestep.setInterval(100);

    // We connect the game's timestep `timeout` function to our `game_interface`,
    // specifically the `check_game_object_interactions` function
    QObject::connect(&game_timestep,
                     &QTimer::timeout,
                     &game_interface,
                     &GameInterface::increment_game_step);

    // Start our game timestep
    game_timestep.start();

    // execute the event loop. Return the result of the event loop if it stops.

    return a.exec();
}
