#ifndef GAMEINTERFACE_H
#define GAMEINTERFACE_H

#include <QObject>
#include <QTimer>
#include <QVariant>

class Player;
class Food;
class Virus;


class GameInterface : public QObject
{
    // `Q_OBJECT` is a macro so that we can use signals and slots on this class
    Q_OBJECT
    Q_PROPERTY(QVariantList food READ get_food NOTIFY update_food)
    Q_PROPERTY(QVariantList players READ get_players NOTIFY update_players)
    Q_PROPERTY(QVariantList viruses READ get_viruses NOTIFY update_viruses)

public:
    explicit GameInterface(QObject *parent = nullptr);
    QVariantList get_food();
    QVariantList get_viruses();
    QVariantList get_players();
    void track_food_fired_by_players(Food *new_food);
    void track_new_virus(Virus *virus);
    void remove_virus_from_game(Virus *virus);
    QRect *game_size();
    void start_game();
    void add_player(Player* player);

    void set_game_size(int width, int height);
    Q_INVOKABLE Player* get_this_player(QString authentication);

public slots:
    void increment_game_step();
    void remove_player(Player *player);
    // void set_game_height(int height);
    // void set_game_width(int width);

protected:
    void create_game_objects();
    void create_viruses(int number=5);
    void create_food(int number=500);
    void check_game_object_interactions();

signals:
    void update_food();
    void update_viruses();
    void update_players();
    void update_this_player();

private:
    QVariantList _food;
    QVariantList _viruses;
    QVariantList _players;
    QRect *_game_size;
    QTimer _game_interval;
};

#endif // GAMEINTERFACE_H
