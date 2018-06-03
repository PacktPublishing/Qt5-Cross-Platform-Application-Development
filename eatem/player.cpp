#include "player.h"

Player::Player(QObject *parent) : QObject(parent)
  , _x(100)
  , _y(100)
  , _mass(20)
{
}

int Player::get_radius()
{
    return 4 + qSqrt(_mass) * 6;
}

void Player::validate_coordinates()
{
    if (_x < 0)
        _x = 0;
    if (_y < 0)
        _y = 0;
    // todo: add in window coordinates


}

void Player::request_coordinates(int x, int y)
{
    if (x == 0 && y == 0)
        return;
    qDebug() << x << y;
    int distance = qSqrt(pow(x, 2) + pow(y, 2));
    // prevents jitter
    if (distance < 3)
        return;

    // Player [x, y] - mouse [x, y]
    int target_x = _x - x;
    int target_y =  _y - y;


    qreal radians = atan2(target_y, target_x);
    // FIXME: configurable?
    int slowdown = 1;

    // calc deltas
    qreal delta_y = 3 * sin(radians) / slowdown;
    qreal delta_x = 3 * cos(radians) / slowdown;

    // move player
    _x -= qRound(delta_x);
    _y -= qRound(delta_y);
    validate_coordinates();
}
