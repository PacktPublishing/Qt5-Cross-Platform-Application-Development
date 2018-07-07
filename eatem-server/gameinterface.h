#ifndef GAMEINTERFACE_H
#define GAMEINTERFACE_H

#include <QObject>

#include <QWebChannel>
#include <QWebSocketServer>
#include "websockettransport.h"

// `Player` class
//    Complex class that manages our player
#include "player.h"

// `Virus` class
//     simple class that stores attributes (x/y coordinates, radius) for our virus
#include "virus.h"

// `Food` class
//     simple class that stores attributes (x/y coordinates, color) for our food
#include "food.h"


// Need to declare these pointers as Qt metatypes using the
// `Q_DECLARE_METATYPE` macro so that we can add them into `QVariants`.
// We need to add them into `QVaraint` so that we can pass them to QML
// in a `QVariantList`. `QVariantList` works as a list in JavaScript

Q_DECLARE_METATYPE(Virus *)
Q_DECLARE_METATYPE(Food *)
Q_DECLARE_METATYPE(Player *)

// NOTE: `QVaraint` will NOT take object values, hence the use of pointers here


// `GameInterface` class
//    is an interface that abstracts away some of the game intialization logic for us.
//    Additionally responsible for incrementing the game timestep.
//    NOTE: If the game was more complex, we'd break the timestep
//    logic out separately of the interface
class GameInterface : public QObject
{
    // `Q_OBJECT` is a macro so that we can use signals and slots on this class

    Q_OBJECT

public:
    explicit GameInterface(QObject *parent = nullptr);
    QVariantList get_food();
    QVariantList get_viruses();
    QVariantList get_players();
    // NOTE: This will go away
    Player* get_this_player();

public slots:
    void increment_game_step();

protected slots:
    void handle_new_connection();

signals:
    void client_connected(WebSocketTransport *client);

protected:
    void create_viruses();
    void create_food(int number=500);
    void check_game_object_interactions();

private:
    QVariantList _food;
    QVariantList _viruses;
    QVariantList _players;
    QRect _game_size;

    // The Web Channel
    QWebChannel *_webchannel;

    // Web Socket Server
    QWebSocketServer *_websocket_server;
};

#endif // GAMEINTERFACE_H
