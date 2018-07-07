#include <QCoreApplication>
#include <QWebSocketServer>

#include <QTimer>

#include <QObject>
#include <QVariantList>

#include "gameinterface.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // http://doc.qt.io/qt-5/qtwebchannel-chatclient-qml-qmlchatclient-qml.html

    // Create our `GameInterface`, an interface which abstracts some of the game creation
    // aspects for us,  including:
    // 1. creating food
    // 2. creating viruses
    // 3. Creating a list of players
    GameInterface game_interface;


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
