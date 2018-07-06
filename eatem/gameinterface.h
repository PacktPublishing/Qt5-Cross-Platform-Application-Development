#ifndef GAMEINTERFACE_H
#define GAMEINTERFACE_H

#include <QObject>

// Provides the `Player` class
// More complex type that manages our player's interactions
#include "player.h"

// Provides the `Virus` class
// simple class that stores attributes (x/y coordinates, radius) for our virus
#include "virus.h"

// Provides the `Food` class
// simple class that stores attributes (x/y coordinates, color) for our food
#include "food.h"


// Need to declare these pointers as Qt metatypes using the
// `Q_DECLARE_METATYPE` macro so that we can add them into `QVariants`.
// We need to add them into `QVaraint` so that we can pass them to QML
// in a `QVariantList`. `QVariantList` works as a list in JavaScript
Q_DECLARE_METATYPE(Virus *)
Q_DECLARE_METATYPE(Food *)
Q_DECLARE_METATYPE(Player *)
// Note that the `QVaraint` will NOT take object values, hence the use of pointers


// `GameInterface` is an interface that abstracts away some of the game intialization logic for us.
// Also responsible for incrementing the game timestep. If the game was more complex, we'd break that
// functionality out seperate of this class
class GameInterface : public QObject
{
    // Macro so that we can use signals and slots on this class
    Q_OBJECT
public:
    // FIXME: Probably won't pass in the `window_size`
    explicit GameInterface(QRect window_size, QObject *parent = nullptr);
    QVariantList get_food();
    QVariantList get_viruses();
    QVariantList get_players();
    Player* get_this_player();

public slots:
    void check_game_object_interactions();

    // FIXME: this is probably going away
    void debug(int height)
    {
        qDebug() << height;
    }

protected:
    void create_viruses();
    void create_food(int number=500);

private:
    Player* _this_player;
    QVariantList _food;
    QVariantList _viruses;
    QVariantList _players;
    QRect _game_size;
};

#endif // GAMEINTERFACE_H
