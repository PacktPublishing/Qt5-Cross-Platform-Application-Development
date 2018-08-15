#ifndef GAMEINTERFACE_H
#define GAMEINTERFACE_H

#include <QObject>
#include <QTimer>
#include <QVariant>
#include <QQmlListProperty>

class Player;
class Food;
class Virus;
class Cell;


// https://stackoverflow.com/questions/26398932/running-into-issues-with-qvariantlists-and-qlists-of-custom-qobjects
class GameInterface : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<Food> food READ food NOTIFY food_changed)
    Q_PROPERTY(QQmlListProperty<Player> players READ players NOTIFY players_changed)
    Q_PROPERTY(QQmlListProperty<Virus> viruses READ viruses NOTIFY viruses_changed)

public:
    explicit GameInterface(QObject *parent = nullptr);

    QQmlListProperty<Food> food();
    QQmlListProperty<Virus> viruses();
    QQmlListProperty<Player> players();

    void track_food_fired_by_players(Food *new_food);
    void track_new_virus(Virus *virus);
    void remove_virus_from_game(Virus *virus);
    QRect *game_size();
    void start_game();
    void add_player(Player* player);

    void set_game_size(int width, int height);
    Q_INVOKABLE QVariant get_player(QString authentication);

public slots:
    void increment_game_step();
    void remove_player(Player *player);

protected:
    void create_game_objects();
    void create_viruses(int number=5);
    void create_food(int number=500);
    void check_game_object_interactions();

    bool _check_virus_interactions(Food* food);
    bool _check_player_interactions(Food* food);

signals:
    void food_changed();
    void viruses_changed();
    void players_changed();

private:
    QList<Food *> _food;
    QList<Virus *> _viruses;
    QList<Player *> _players;

    QRect *_game_size;
    QTimer _game_interval;
};

#endif // GAMEINTERFACE_H
