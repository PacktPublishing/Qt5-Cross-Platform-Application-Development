#ifndef GAMEINTERFACE_H
#define GAMEINTERFACE_H

#include <QObject>

#include "virus.h"
#include "food.h"
#include "player.h"

// Need to declare these pointers as metatypes
// so we can add them into `QVariants`.
// Need to add them into `QVaraints` so that we can pass them to QML.
Q_DECLARE_METATYPE(Virus *)
Q_DECLARE_METATYPE(Food *)
Q_DECLARE_METATYPE(Player *)

class GameInterface : public QObject
{
    Q_OBJECT
public:
    explicit GameInterface(QRect window_size, QObject *parent = nullptr);
    QVariantList get_food();
    QVariantList get_viruses();
    QVariantList get_players();
    Player* get_this_player();

public slots:
    void check_game_object_interactions();
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
