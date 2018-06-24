#include "cell.h"
#include <QtMath>
#include <QDebug>

Cell::Cell(QObject *parent, int initial_velocity)
    : QObject(parent)
    , _x(100)
    , _y(100)
    // FIXME: have both these numbers tied together
    , _mass(20)
    , _initial_mass(20)
    , _velocity(initial_velocity)
    , _friendly_cell_touching(false)
{
    if (initial_velocity> 0)
    {
        // FIXME: this should set at the NHA
        // FIXME: can also pass in a timer type.
        // default is "coarse"
        _timer_id = startTimer(100);
    }
}

int Cell::radius()
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

int Cell::x()
{
    return _x;
}

int Cell::y()
{
    return _y;
}

qreal Cell::calc_radians(int x, int y)
{
    // NOTE: this works for second firing
    /*
    if (_timer_id > 0)
        return;
    */

    // Cell [x, y] - mouse [x, y]
    int target_x = _x - x;
    int target_y =  _y - y;
    if (target_x == 0 && target_y == 0)
        return 0;

    int distance = qSqrt(pow(target_x, 2) + pow(target_y, 2));
    // prevents jitter
    if (distance < 3)
        return 0;

    return atan2(target_y, target_x);

}

void Cell::request_coordinates(int x, int y)
{

    qreal radians = get_radians(x, y);

    // calc deltas
    qreal delta_y = 3 * sin(radians);
    qreal delta_x = 3 * cos(radians);

    // move cell
    _x -= qRound(delta_x);
    _y -= qRound(delta_y);

    // ensure we don't run off the map
    validate_coordinates();
}

bool Cell::is_object_touching(int object_x, int object_y)
{
    int diff_x = _x - object_x;
    int diff_y = _y - object_y;
    float distance = pow((pow(diff_x, 2) + pow(diff_y, 2)), 0.5);
    return distance <= radius();
}

void Cell::timerEvent(QTimerEvent *event)
{
    if (_velocity <= 0)
    {
        // FIXME: shut off timer
        _timer_id;
        return;
    }
}

QPointer<Cell> Cell::request_split(int mouse_x, int mouse_y, QObject *new_cell_parent)
{
    QPointer<Cell> result;
    int two_times_intial = 2 * _initial_mass;
    if (_mass > two_times_intial)
    {
        // FIXME: add in velocity?
        Cell *split_cell = new Cell(new_cell_parent);
        result = split_cell;
        _mass /= 2;
    }

    return result;
}
