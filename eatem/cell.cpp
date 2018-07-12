#include "cell.h"
#include <QtMath>
#include <QDebug>
#include <QRandomGenerator>
#include "player.h"

Cell::Cell(QRect *game_size, QObject *parent)
    : QObject(parent)
    , _mass(Cell::initial_mass)
    , _game_size(game_size)
    , _velocity_ticks(-1)
{
    QRandomGenerator random = QRandomGenerator::securelySeeded();
    if (_game_size->width() == 0)
        _position = QVector2D(random.bounded(1000), random.bounded(1000));
    else
        _position = QVector2D(random.bounded(_game_size->width()), random.bounded(_game_size->height()));
}

Cell::Cell(QVector2D start_position, QVector2D velocity, qreal mass, QRect *game_size, QObject *parent)
    : QObject(parent)
    , _mass(mass)
    , _position(start_position)
    , _game_size(game_size)
    , _velocity(velocity)
    , _velocity_ticks(10)
{
}

qreal Cell::radius()
{
    return qSqrt(_mass/M_PI);
}

void Cell::validate_coordinates()
{
    // Check to see if we're within the game width
    if (_position.x() < 0)
        _position.setX(0);
    // Check to see if we're within the game width
    else if (_position.x() > _game_size->width())
        _position.setX(_game_size->width());

    // Check to see if we're within the game height
    if (_position.y() < 0)
        _position.setY(0);
    // Check to see if we're within the game height
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

void Cell::request_coordinates(QVector2D mouse_position)
{
    if (abs(mouse_position.x()-_position.x()) <= 1 && abs(mouse_position.y()-_position.y()) <= 1)
        return;

    QVector2D target = mouse_position - _position;
    target.normalize();
    // FIXME: slow down with mass
    target *= 3;

    if (_velocity_ticks > 0)
    {
        // target += _velocity;
        _velocity_ticks -= 1;
    }

    _position += target;

    // ensure we don't run off the map
    validate_coordinates();
}

QVector2D Cell::position()
{
    return _position;
}

void Cell::request_coordinates(QVector2D target_position, Cell *touching_cell)
{
    // https://gamedev.stackexchange.com/questions/74872/how-to-solve-the-overlap-of-two-circles-that-collide
    // https://gamedev.stackexchange.com/questions/160485/subtracting-magnitude-from-2d-circle-contact/160489#160489
    QVector2D to_target = target_position - _position;
    // TODO: calculate me
    QVector2D normal = (_position - touching_cell->position()).normalized();
    QVector2D target_normalized = to_target.normalized();

    float into_collision = qMin(QVector2D::dotProduct(target_normalized, normal), (float) 0);
    if (into_collision == 0)
    {
        to_target.setX(-to_target.x());
        into_collision = QVector2D::dotProduct(to_target, normal);
    }

    QVector2D to_target_non_collide = (to_target - normal) * into_collision;
    // FIXME: add in some sort of clamp down to 3.
    // or could add in as an acceleration, to avoid the zig-zag
    _position += to_target_non_collide.normalized()*3;

    // Distance between the cells
    // float distance = abs(_position.distanceToPoint(touching_cell->position()));

    // half the amount of overlap
    // float overlap = 0.5 * (distance  - radius() - touching_cell->radius());

    // FIXME: adjust this value?
    // float push_velocity = qMax(overlap, 5.);
    // Normal is (c2.center - c1.center) for c1
    // QVector2D push_vector = (_position - touching_cell->position()).normalized();

    if (_velocity_ticks > 0)
    {
        // push_vector += _velocity;
        _velocity_ticks -= 1;
    }
    // Ok now we need to remove all the magnitude in a certain direction
    // https://math.stackexchange.com/questions/598685/point-deflecting-off-of-a-circle?noredirect=1&lq=1

    // move cell
    validate_coordinates();
}

void Cell::request_coordinates(QVector2D position, QList<Cell *> touching_cells)
{
    qDebug() << "Made it here";
    if (touching_cells.isEmpty())
    {
        request_coordinates(position);
    }


}

bool Cell::is_object_touching(QVector2D other_center, int object_radius)
{
    int radiuses = qPow(object_radius + radius(), 2);
    int diff_x = qPow(_position.x() - other_center.x(), 2);
    int diff_y = qPow(_position.y() - other_center.y(), 2);

    return radiuses > diff_x + diff_y;
}

bool Cell::is_object_touching(QPoint center_point, int object_radius)
{
    // FIXME: clean up naming
    int radiuses = qPow(object_radius + radius(), 2);
    int diff_x = qPow(_position.x() - center_point.x(), 2);
    int diff_y = qPow(_position.y() - center_point.y(), 2);

    return radiuses > diff_x + diff_y;
}

QPointer<Cell> Cell::request_split(QVector2D mouse_position)
{
    QPointer<Cell> result;
    int two_times_intial = 2 * Cell::initial_mass;
    if (_mass > two_times_intial)
    {
        QVector2D target = _position - mouse_position;

        target.normalize();
        QVector2D normalized_target(target);
        target *= 2;

        // FIXME: think about pushing cell half the radius to the right
        Cell *split_cell = new Cell(_position, target, _mass/2, _game_size, parent());

        _position += normalized_target;

        // QVector2D start_position, QVector2D velocity, qreal mass, QRect *game_size, QPlayer *owning_player
        result = split_cell;

        _mass /= 2;
    }

    return result;
}
