#include "gameinterface.h"
#include "player.h"
#include "virus.h"
#include "food.h"
#include "cell.h"
#include <QDebug>


// Need to declare these pointers as Qt metatypes using the
// `Q_DECLARE_METATYPE` macro so that we can add them into `QVariants`.
// We need to add them into `QVaraint` so that we can pass them to QML
// in a `QVariantList`. `QVariantList` works as a list in JavaScript

Q_DECLARE_METATYPE(QQmlListProperty<Player>)
Q_DECLARE_METATYPE(QQmlListProperty<Food>)
Q_DECLARE_METATYPE(QQmlListProperty<Virus>)

Q_DECLARE_METATYPE(Player *)
Q_DECLARE_METATYPE(Virus *)
Q_DECLARE_METATYPE(Food *)
// NOTE: `QVaraint` will NOT take object values, hence the use of pointers here

GameInterface::GameInterface(QObject *parent)
    : QObject(parent)
    , _game_size(new QRect())
{
    _game_interval.setInterval(50);
    connect(&_game_interval, &QTimer::timeout, this, &GameInterface::increment_game_step);
}

void GameInterface::set_game_size(int width, int height)
{
    _game_size->setHeight(height);
    _game_size->setWidth(width);
}

void GameInterface::create_viruses(int number)
{
    for(int i=0; i < number; i++) {
        Virus *virus = new Virus(_game_size, this);
        _viruses.append(virus);
    }
}

void GameInterface::create_game_objects()
{
    create_food(1000);
    create_viruses();
    emit food_changed();
    emit viruses_changed();
}


void GameInterface::create_food(int number)
{
    for(int i=0; i<number; i++)
    {
        Food *food = new Food(_game_size);
        _food.append(food);
    }
}

QQmlListProperty<Food> GameInterface::food()
{
    return QQmlListProperty<Food>(this, _food);
}

QQmlListProperty<Virus> GameInterface::viruses()
{
    return QQmlListProperty<Virus>(this, _viruses);
}

QVariant GameInterface::get_player(QString authentication)
{
    for (Player *player : _players)
    {
        if (player->authentication() == authentication)
        {
            return QVariant::fromValue<Player *>(player);
        }
    }
    return QVariant();
}


QQmlListProperty<Player> GameInterface::players()
{
    return QQmlListProperty<Player>(this, _players);
}

void GameInterface::increment_game_step()
{
    check_game_object_interactions();
}

void GameInterface::remove_player(Player *player)
{
    _players.removeAll(player);
    player->deleteLater();
    emit players_changed();
}

bool GameInterface::_check_player_interactions(Food *food)
{
    // For each Player QVariant in our QVariantList `_players`...
    for(Player *player : _players){
        for (Cell *cell: player->internal_cell_list())
        {
            if (cell->is_touching(food->ball_properties()))
            {
                cell->eat_food(food);
                food->set_enabled(false);
                return true;
            }
        }
    }
    return false;
}

bool GameInterface::_check_virus_interactions(Food *food)
{
    for (Virus *virus : _viruses) {
        if (virus->is_touching(food->ball_properties()))
        {
            virus->eat_food(food);
            food->set_enabled(false);
            return true;
        }
    }

    return false;
}

void GameInterface::check_game_object_interactions()
{
    if (_viruses.empty() and _food.empty())
        return;

    for (Food *food : _food) {
        bool food_disabled = food->is_disabled();

        if (food_disabled)
            continue;

        food_disabled = _check_virus_interactions(food);

        if (food_disabled)
            continue;

        _check_player_interactions(food);
    }

    // For each Player in our list `_players`...
    for(Player *player : _players)
    {
        // Now iterate through every virus variant
        for (Virus *virus : _viruses)
        {
            player->handle_touch(virus);
        }

        // Now iterate through every other player variant
        for (Player *other_player : _players)
        {
            if (player == other_player)
                continue;
            player->handle_touch(other_player);
        }
    }
}

void GameInterface::track_food_fired_by_players(Food *new_food)
{
    _food.append(new_food);
    emit food_changed();
}

void GameInterface::track_new_virus(Virus *virus)
{
    _viruses.append(virus);
    viruses_changed();
}

void GameInterface::remove_virus_from_game(Virus *virus)
{
    _viruses.removeOne(virus);
}

QRect *GameInterface::game_size()
{
    return _game_size;

}

void GameInterface::start_game()
{
    // FIXME: add in checks for game size!
    create_game_objects();
    _game_interval.start();
}

void GameInterface::add_player(Player *player)
{
    _players.append(player);
    emit players_changed();
}
