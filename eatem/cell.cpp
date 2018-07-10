#include "cell.h"
#include <QtMath>
#include <QDebug>
#include <QRandomGenerator>
#include "player.h"

Cell::Cell(QRect *game_size, QObject *parent, int initial_velocity)
    : QObject(parent)
    , _mass(Cell::initial_mass)
    , _velocity(initial_velocity)
    , _game_size(game_size)
{
    QRandomGenerator random = QRandomGenerator::securelySeeded();
    if (_game_size->width() == 0 && initial_velocity == 0)
        _position = QVector2D(random.bounded(1000), random.bounded(1000));
    else if (initial_velocity == 0)
        _position = QVector2D(random.bounded(_game_size->width()), random.bounded(_game_size->height()));
    // else

    if (initial_velocity > 0)
    {
        // NOTE: can also pass in a timer type.
        // default is "coarse"
        _timer_id = startTimer(100);
    }

}

int Cell::radius()
{
    return qSqrt(_mass/M_PI);
}

void Cell::validate_coordinates()
{
    if (_position.x() < 0)
        _position.setX(0);
    else if (_position.x() > _game_size->width())
        _position.setX(_game_size->width());

    if (_position.y() < 0)
        _position.setY(0);
    else if (_position.y() > _game_size->height())
        _position.setY(_game_size->height());
}

void Cell::add_mass(qreal amount)
{
   _mass += amount;
}

qreal Cell::mass()
{
    return _mass;
}

int Cell::x()
{
    return _position.x();
}

int Cell::y()
{
    return _position.y();
}

/*
qreal Cell::calc_radians(int x, int y)
{
    // NOTE: this works for second firing
    if (_timer_id > 0)
        return;

    // Cell [x, y] - mouse [x, y]
    QVector2D mouse(x, y);
    QVector2D target = _position - mouse;
    if (target.x() == 0 && target.y() == 0)
        return 0;

    int distance = target.length();
    // FIXME: use deterministic physics?
    // prevents jitter
    if (distance < 3)
        return 0;

    return atan2(target.y(), target.x());

}
*/

void Cell::request_coordinates(int x, int y)
{
    if (abs(x-_position.x()) <= 1 && abs(y-_position.y()) <= 1)
        return;

    QVector2D mouse(x, y);
    QVector2D target = mouse - _position;
    target.normalize();
    // FIXME: slow down with mass
    target *= 3;

    /*
    qreal radians = calc_radians(x, y);

    // calc deltas
    qreal delta_y = 3 * sin(radians);
    qreal delta_x = 3 * cos(radians);

    // NOTE: delta is the velocity!

    // move cell
    _position.setX(_position.x() - qRound(delta_x));
    _position.setY(_position.y() - qRound(delta_y));
    */

    _position += target;

    // ensure we don't run off the map
    validate_coordinates();
}

QVector2D Cell::position()
{
    return _position;
}

void Cell::request_coordinates(int x, int y, Cell *touching_cell)
{
    // https://gamedev.stackexchange.com/questions/74872/how-to-solve-the-overlap-of-two-circles-that-collide
    // QVector2D target(x, y);

    // Distance between the cells
    float distance = abs(_position.distanceToPoint(touching_cell->position()));
    // half the amount of overlap
    // NOTE: may need to upper bound this?
    float overlap = 0.5 * (distance  - radius() - touching_cell->radius());
    QVector2D push_vector = (_position - touching_cell->position()).normalized() * overlap;

    // Ok now we need to remove all the magnitude in a certain direction
    // https://math.stackexchange.com/questions/598685/point-deflecting-off-of-a-circle?noredirect=1&lq=1

    // Find the point of intersection

    // QVector2D mouse_velocity(3*cos(radians), 3*sin(radians));
    //_position -= (mouse_velocity - push_vector).normalized() * 3;
    _position -= push_vector;
    // touching_cells->position() += (mouse_velocity - push_vector).normalized() * 3;

    // move cell
    validate_coordinates();
}

void Cell::request_coordinates(int x, int y, QList<Cell *> touching_cells)
{
    if (touching_cells.isEmpty())
    {
        request_coordinates(x, y);
    }


}

bool Cell::is_object_touching(int object_x, int object_y, int object_radius)
{
    // FIXME: clean up naming
    int radiuses = qPow(object_radius + radius(), 2);
    int diff_x = qPow(_position.x() - object_x, 2);
    int diff_y = qPow(_position.y() - object_y, 2);

    return radiuses > diff_x + diff_y;

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

QPointer<Cell> Cell::request_split(int mouse_x, int mouse_y, Player *owning_player)
{
    QPointer<Cell> result;
    int two_times_intial = 2 * Cell::initial_mass;
    if (_mass > two_times_intial)
    {
        // FIXME: add in velocity?
        Cell *split_cell = new Cell(_game_size, owning_player);
        result = split_cell;
        _mass /= 2;
    }

    return result;
}
