#ifndef UTIL_H
#define UTIL_H

#include <QtMath>

class Player;
class Food;
class Virus;


inline bool touching(Player *player, Food *food)
{
    int fudge_factor = 5;
    return player->touching(food, fudge_factor);
}

inline bool touching(Player *player, Virus *virus)
{
    int fudge_factor = 5;
    return _touching_helper(player->get_x(),
                            player->get_y(),
                            virus->get_x(),
                            virus->get_y(),
                            player->get_radius() + fudge_factor);
}

inline bool touching(Player *player, Player *other_player)
{
    // TODO: change this
    int fudge_factor = 5;
    return _touching_helper(player->get_x(),
                            player->get_y(),
                            other_player->get_x(),
                            other_player->get_y(),
                            player->get_radius() + fudge_factor);

}

#endif // UTIL_H
