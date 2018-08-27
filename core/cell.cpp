#include "cell.h"

#include <QtMath>
#include <QDebug>
#include <QRect>
#include <QRandomGenerator>

#include "player.h"
#include "food.h"
#include "ball.h"

Cell::Cell(QRect *game_size, QObject *parent)
    : QObject(parent)
    , _ball_properties(new Ball(game_size, Cell::initial_mass, this))
{
    _ball_properties->set_coordinates_random();
    _connect_ball_property_signals();
}

Cell::Cell(Ball *ball_properties, QObject *parent)
    : QObject(parent)
    , _ball_properties(ball_properties)
{
    _connect_ball_property_signals();
    _ball_properties->setParent(this);
}

void Cell::_connect_ball_property_signals()
{
    connect(_ball_properties, &Ball::x_changed, this, &Cell::x_changed);
    connect(_ball_properties, &Ball::y_changed, this, &Cell::y_changed);
    connect(_ball_properties, &Ball::radius_changed, this, &Cell::radius_changed);
}

qreal Cell::radius()
{
    return _ball_properties->radius();
}

void Cell::add_mass(qreal amount)
{
    _ball_properties->add_mass(amount);
}

qreal Cell::mass()
{
    return _ball_properties->mass();
}

int Cell::x()
{
    return _ball_properties->x();
}

int Cell::y()
{
    return _ball_properties->y();
}

void Cell::eat_food(Food *food)
{
    _ball_properties->add_mass(food->mass());
}

void Cell::request_coordinates(QPoint mouse_position)
{
    _ball_properties->request_coordinates(mouse_position);
}

void Cell::request_coordinates(QPoint target_position, Ball *touching_ball)
{
    _ball_properties->request_coordinates(target_position, touching_ball);
}

QPoint Cell::position()
{
    return _ball_properties->position();
}


Ball *Cell::ball_properties()
{
    return _ball_properties;
}

void Cell::set_mass(qreal mass)
{
    _ball_properties->set_mass(mass);
}

void Cell::move()
{
    _ball_properties->move();
}

void Cell::request_coordinates(QPoint position, QList<Cell *> touching_cells)
{
    if (touching_cells.isEmpty())
        return request_coordinates(position);

    QList<Ball *> touching_balls;

    for (Cell* cell : touching_cells)
        touching_balls.append(cell->ball_properties());

    return _ball_properties->request_coordinates(position, touching_balls);
}

bool Cell::is_touching(Ball *other)
{
    return _ball_properties->is_touching(other);
}

QPointer<Cell> Cell::request_split(QPoint mouse_position)
{
    QPointer<Cell> result;
    int two_times_intial = 2 * Cell::initial_mass;
    if (_ball_properties->mass() > two_times_intial)
    {
        QPoint target_point = mouse_position - _ball_properties->position();

        QVector2D target = QVector2D(target_point).normalized();

        // QVector2D normalized_target(target);
        target *= 10;

        qreal new_mass = _ball_properties->mass()/2;

        Ball *new_ball = new Ball(*_ball_properties);
        new_ball->set_initial_velocity(target);
        new_ball->set_mass(new_mass);
        new_ball->set_velocity_ticks(30);

        // FIXME: think about pushing cell half the radius to the right
        Cell *split_cell = new Cell(new_ball, parent());

        // FIXME: verify this works without, see line 118
        // _position += normalized_target;

        // QVector2D start_position, QVector2D velocity, qreal mass, QRect *game_size, QPlayer *owning_player
        result = split_cell;
        _ball_properties->set_mass(new_mass);
    }

    return result;
}

QPointer<Food> Cell::request_fire_food(QPoint mouse_position)
{
    QPointer<Food> result;
    qreal requested_mass = Food::initial_mass * 20;
    if (_ball_properties->mass() - requested_mass > Cell::initial_mass)
    {
        QVector2D to_target = QVector2D(mouse_position - _ball_properties->position()).normalized();
        QVector2D start(to_target);
        start *= radius();
        start *= 1.3;

        // Need to put the new food starting position outside of the
        // current cell
        QPoint starting_position(_ball_properties->x() + start.x(), _ball_properties->y() + start.y());
        to_target *= 10;

        Ball *new_ball = new Ball(ball_properties()->game_size(), to_target, starting_position, requested_mass);
        new_ball->set_velocity_ticks(30);

        Food *new_food = new Food(new_ball);
        result = new_food;
        _ball_properties->remove_mass(requested_mass);
    }

    return result;
}
