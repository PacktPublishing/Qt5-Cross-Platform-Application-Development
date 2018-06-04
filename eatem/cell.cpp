#include "cell.h"

Cell::Cell(QObject *parent) : QObject(parent)
  , _x(100)
  , _y(100)
  , _mass(20)
{
}

int Cell::get_radius()
{
    return 4 + qSqrt(_mass) * 6;
}

void Cell::validate_coordinates()
{
    if (_x < 0)
        _x = 0;
    if (_y < 0)
        _y = 0;
    // todo: add in window coordinates
}

void Cell::add_mass(int amount)
{
   _mass += amount;
}

int Cell::get_mass()
{
    return _mass;
}

int Cell::get_x()
{
    return _x;
}

int Cell::get_y()
{
    return _y;
}

void Cell::request_coordinates(int x, int y)
{
    // Cell [x, y] - mouse [x, y]
    int target_x = _x - x;
    int target_y =  _y - y;
    if (target_x == 0 && target_y == 0)
        return;

    int distance = qSqrt(pow(target_x, 2) + pow(target_y, 2));
    // prevents jitter
    if (distance < 3)
        return;



    qreal radians = atan2(target_y, target_x);
    // FIXME: configurable?
    int slowdown = 1;

    // calc deltas
    qreal delta_y = 3 * sin(radians) / slowdown;
    qreal delta_x = 3 * cos(radians) / slowdown;

    // move cell
    _x -= qRound(delta_x);
    _y -= qRound(delta_y);

    // insure we don't run off the map
    validate_coordinates();
}
