#include "cell.h"
#include <QtMath>
#include <QDebug>
#include <QRandomGenerator>
#include <QRect>
#include "player.h"
#include "food.h"

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

    if (_velocity_ticks > 0) {
        target += _velocity;
        _velocity_ticks -= 1;
    }
    else {
        target *= 3;
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
        to_target.setX(-target_normalized.x());
        into_collision = QVector2D::dotProduct(target_normalized, normal);
    }

    QVector2D to_target_non_collide = (to_target - normal) * into_collision;

    if (_velocity_ticks > 0)
    {
        // FIXME: rename `_velocity`
        // NOTE: `to_target_non_collide` is not normalized here
        qDebug() << _velocity << to_target_non_collide << _position;
        to_target_non_collide += _velocity;
        _velocity_ticks -= 1;
    }
    else
    {
        to_target_non_collide.normalize();
        to_target_non_collide *= 3;
    }

    // FIXME: add in some sort of clamp down to 3.
    // or could add in as an acceleration, to avoid the zig-zag
    _position += to_target_non_collide;
    // validate that we haven't run off the map
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
        QVector2D target = mouse_position - _position;

        target.normalize();
        QVector2D normalized_target(target);
        target *= 3;

        // FIXME: think about pushing cell half the radius to the right
        Cell *split_cell = new Cell(_position, target, _mass/2, _game_size, parent());

        _position += normalized_target;

        // QVector2D start_position, QVector2D velocity, qreal mass, QRect *game_size, QPlayer *owning_player
        result = split_cell;

        _mass /= 2;
    }

    return result;
}

QPointer<Food> Cell::request_fire_food(QPoint mouse_position)
{
    QPointer<Food> result;
    qreal requested_mass = Food::initial_mass * 2;
    if (_mass - requested_mass > Cell::initial_mass)
    {
        // FIXME: calculate
        QVector2D intial_velocity;

        // Need to put the new food starting position outside of the
        // current cell
        QPoint starting_position;

        Food *new_food = new Food(intial_velocity, starting_position, requested_mass, _game_size);
        result = new_food;
    }

    return result;
}
