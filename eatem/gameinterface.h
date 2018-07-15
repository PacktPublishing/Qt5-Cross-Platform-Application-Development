#ifndef GAMEINTERFACE_H
#define GAMEINTERFACE_H

#include <QObject>
#include <QTimer>

#include "player.h"
#include "virus.h"
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
    Q_PROPERTY(QVariantList food READ get_food NOTIFY update_food)
    Q_PROPERTY(QVariantList players READ get_players NOTIFY update_players)
    Q_PROPERTY(QVariantList viruses READ get_viruses NOTIFY update_viruses)
    // FIXME: Get rid of this
    Q_PROPERTY(Player* this_player READ get_this_player NOTIFY update_this_player)

public:
    explicit GameInterface(QObject *parent = nullptr);
    QVariantList get_food();
    QVariantList get_viruses();
    QVariantList get_players();
    void track_food_fired_by_players(Food *new_food);

    // NOTE: The invokeableness of this will go away
    Q_INVOKABLE void set_game_size(int width, int height);

    // FIXME: This will go away
    Player* get_this_player();

public slots:
    void increment_game_step();
    // void set_game_height(int height);
    // void set_game_width(int width);

protected:
    void create_viruses(int number=5);
    void create_food(int number=500);
    void check_game_object_interactions();

protected slots:
    void slot_game_object_creation();

signals:
    void create_game_objects();
    void update_food();
    void update_viruses();
    void update_players();
    void update_this_player();

private:
    Player* _this_player;
    QVariantList _food;
    QVariantList _viruses;
    QVariantList _players;
    QRect *_game_size;
    QTimer _game_interval;
};

#endif // GAMEINTERFACE_H
