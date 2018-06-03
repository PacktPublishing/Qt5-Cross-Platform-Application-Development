#ifndef UTIL_H
#define UTIL_H

#include <QtMath>

class Player;
class Food;
class Virus;

inline bool _touching_helper(int x, int y, int other_x, int other_y, int radius)
{
    int diff_x = x - other_x;
    int diff_y = y - other_y;
    float distance = pow((pow(diff_x, 2) + pow(diff_y, 2)), 0.5);
    return distance <= radius;
}

inline bool touching(Player *player, Food *food)
{
    int fudge_factor = 5;
    return _touching_helper(player->get_x(),
                            player->get_y(),
                            food->get_x(),
                            food->get_y(),
                            player->get_radius() + fudge_factor);
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
