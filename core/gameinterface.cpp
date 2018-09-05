#include "gameinterface.h"
#include "player.h"
#include "virus.h"
#include "food.h"
#include "cell.h"
#include <QDebug>
#include <QRect>


// Need to declare these pointers as Qt metatypes using the
// `Q_DECLARE_METATYPE` macro so that we can add them into `QVariants`.
// We need to add them into `QVaraint` so that we can pass them to QML
// in a `QVariantList`. `QVariantList` works as a list in JavaScript

Q_DECLARE_METATYPE(Virus *)
Q_DECLARE_METATYPE(Food *)
Q_DECLARE_METATYPE(Player *)
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
        _viruses.append(QVariant::fromValue<Virus*>(virus));
    }
}

void GameInterface::create_game_objects()
{
    // create_food(40);
    create_food();
    create_viruses(5);
    emit food_changed();
    emit viruses_changed();
}


void GameInterface::create_food(int number)
{
    for(int i=0; i<number; i++)
    {
        Food *food = new Food(_game_size);
        _food.append(QVariant::fromValue<Food*>(food));
    }
}

QVariantList GameInterface::food()
{
    return _food;
}

QVariantList GameInterface::viruses()
{
    return _viruses;
}

Player* GameInterface::get_player(QString authentication)
{
    for (QVariant player_variant : _players)
    {
        Player *player = player_variant.value<Player *>();
        if (player->authentication() == authentication)
        {
            return player;
        }
    }
    return nullptr;
}


QVariantList GameInterface::players()
{
    return _players;
}

void GameInterface::increment_game_step()
{
    check_game_object_interactions();
}

void GameInterface::remove_player(Player *player)
{
    _players.removeOne(QVariant::fromValue<Player*>(player));
    player->deleteLater();
    emit players_changed();
}

bool GameInterface::_check_player_interactions(Food *food)
{
    // For each Player QVariant in our QVariantList `_players`...
    for(QVariant player_variant : _players){
        // cast each player variant into into a `Player` pointer
        Player *player = player_variant.value<Player*>();

        for (QVariant cell_variant : player->cells())
        {
            Cell *cell = cell_variant.value<Cell *>();
            if (cell->is_touching(food->ball_properties()))
            {
                cell->eat_food(food);
                food->deleteLater();
                return true;
            }
        }
    }
    return false;
}

bool GameInterface::_check_virus_interactions(Food *food)
{
    for (QVariant virus_variant : _viruses) {
        Virus *virus = virus_variant.value<Virus *>();
        virus->move();

        if (virus->is_touching(food->ball_properties()))
        {
            virus->eat_food(food);
            food->deleteLater();
            return true;
        }
    }

    return false;
}

void GameInterface::check_game_object_interactions()
{
    QMutableListIterator<QVariant> i(_food);
    while (i.hasNext()) {
        QVariant food_variant = i.next();
        Food *food = food_variant.value<Food *>();
        // NOTE: `_check_virus_interactions` calls `Virus::move()`
        bool food_disabled = _check_virus_interactions(food);

        if (food_disabled)
        {
            i.remove();
            emit food_changed();
            continue;
        }

        food_disabled = _check_player_interactions(food);

        if (food_disabled)
        {
            i.remove();
            emit food_changed();
        }
        else
            food->move();
    }

    // For each Player QVariant in our QVariantList `_players`...
    for(QVariant player_variant : _players)
    {
        // cast each player variant into into a `Player` pointer
        Player *player = player_variant.value<Player*>();

        // Now iterate through every virus variant
        for (QVariant virus_variant: _viruses)
        {
            // cast the virius variant into into a `Virus` pointer
            Virus *virus = virus_variant.value<Virus*>();
            player->handle_touch(virus);
        }

        // Now iterate through every other player variant
        for (QVariant other_player_variant : _players)
        {
            // cast the other player variant into into a `Player` pointer
            Player *other_player = other_player_variant.value<Player*>();
            if (player == other_player)
                continue;
            player->handle_touch(other_player);
        }

        player->move();
    }
}

void GameInterface::track_food_fired_by_players(Food *food)
{
    QVariant food_variant = QVariant::fromValue<Food*>(food);
    _food.append(food_variant);
    emit food_changed();
    emit new_food(food_variant);
}

void GameInterface::track_new_virus(Virus *virus)
{
    QVariant virus_variant = QVariant::fromValue<Virus *>(virus);
    _viruses.append(virus_variant);
    viruses_changed();
    emit new_virus(virus_variant);
}

void GameInterface::remove_virus_from_game(Virus *virus)
{
    _viruses.removeOne(QVariant::fromValue<Virus*>(virus));
    virus->deleteLater();
    emit viruses_changed();
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
    QVariant player_variant = QVariant::fromValue<Player*>(player);
    _players.append(player_variant);
    emit players_changed();
    emit new_player(player_variant);
}
