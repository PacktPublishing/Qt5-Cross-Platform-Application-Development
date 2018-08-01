#include "cell.h"

#include <QtMath>
#include <QDebug>
#include <QRect>
#include <QRandomGenerator>

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
    , _velocity_ticks(30)
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

    emit x_changed();
    emit y_changed();
}

void Cell::add_mass(qreal amount)
{
   _mass += amount;
   emit radius_changed();
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

    if (_velocity_ticks > 0) {
        target += _velocity;
        _velocity_ticks -= 1;
    }
    else {
        target *= velocity();
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
    QVector2D to_target = target_position - _position;
    QVector2D target_normalized = to_target.normalized();

    QVector2D collision_normal = (_position - touching_cell->position()).normalized();
    float into_collision_contribution;

    into_collision_contribution = qMin(QVector2D::dotProduct(target_normalized, collision_normal), (float) 0);
    collision_normal *= into_collision_contribution;

    QVector2D to_target_non_collide = target_normalized - collision_normal;

    if (_velocity_ticks > 0) {
        to_target_non_collide += _velocity;
        _velocity_ticks -= 1;
    }
    else {
        to_target_non_collide *= velocity();
    }

    // FIXME: add in some sort of clamp down to 3.
    // or could add in as an acceleration, to avoid the zig-zag
    _position += to_target_non_collide;

    validate_coordinates();
}

qreal Cell::velocity()
{
    return 3.;
}

void Cell::set_mass(qreal mass)
{
   _mass = mass;
   emit radius_changed();
}

void Cell::request_coordinates(QVector2D position, QList<Cell *> touching_cells)
{
    if (touching_cells.isEmpty())
        return request_coordinates(position);

    QVector2D to_target = position - _position;
    QVector2D target_normalized = to_target.normalized();
    QVector2D to_target_non_collide(target_normalized);

    for (Cell* touching_cell : touching_cells) {
        QVector2D collision_normal = (_position - touching_cell->position()).normalized();
        float into_collision_contribution;
        into_collision_contribution = qMin(QVector2D::dotProduct(target_normalized, collision_normal), (float) 0);
        collision_normal *= into_collision_contribution;
        to_target_non_collide -= collision_normal;
    }

    if (_velocity_ticks > 0) {
        to_target_non_collide += _velocity;
        _velocity_ticks -= 1;
    }
    else {
        to_target_non_collide *= velocity();
    }

    _position += to_target_non_collide;
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
        target *= 10;

        // FIXME: think about pushing cell half the radius to the right
        Cell *split_cell = new Cell(_position, target, _mass/2, _game_size, parent());

        _position += normalized_target;

        // QVector2D start_position, QVector2D velocity, qreal mass, QRect *game_size, QPlayer *owning_player
        result = split_cell;

        _mass /= 2;
        emit radius_changed();
    }

    return result;
}

QPointer<Food> Cell::request_fire_food(QVector2D mouse_position, QColor player_hue)
{
    QPointer<Food> result;
    qreal requested_mass = Food::initial_mass * 20;
    if (_mass - requested_mass > Cell::initial_mass)
    {
        QVector2D to_target = (mouse_position - _position).normalized();
        QVector2D start(to_target);
        start *= radius();
        start *= 1.3;

        // Need to put the new food starting position outside of the
        // current cell
        QPoint starting_position(_position.x() + start.x(), _position.y() + start.y());
        to_target *= 10;
        Food *new_food = new Food(to_target, starting_position, requested_mass, _game_size, player_hue);
        result = new_food;
        _mass -= requested_mass;
    }

    return result;
}
