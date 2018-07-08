#ifndef GAMEINTERFACE_H
#define GAMEINTERFACE_H

#include <QObject>

#include <QWebChannel>
#include <QWebSocketServer>
#include <QRandomGenerator>
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


// Use an anymous function
namespace {

    // https://stackoverflow.com/questions/18862963/qt-c-random-string-generation
    QString GetRandomString()
    {
       const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
       const int randomStringLength = 12; // assuming you want random strings of 12 characters
       const int length_characters = possibleCharacters.length();

       QRandomGenerator random = QRandomGenerator::securelySeeded();

       QString randomString;
       for(int i=0; i<randomStringLength; ++i)
       {
           int index = random.bounded(length_characters);
           QChar nextChar = possibleCharacters.at(index);
           randomString.append(nextChar);
       }

       return randomString;
    }
}


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
    // FIXME: Implement
    Q_INVOKABLE int get_player_index(QString authentication);

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
