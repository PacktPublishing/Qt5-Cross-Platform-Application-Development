#include "food.h"

#include <QRect>
#include <QtMath>
#include <QRandomGenerator>
#include <QDebug>
#include "ball.h"

Food::Food(QRect *game_size, QObject *parent)
    : QObject(parent)
    , _ball_properties(new Ball(game_size, Food::initial_mass, this))
    , _enabled(true)
{
    _ball_properties->set_coordinates_random();
    _connect_ball_property_signals();
}

Food::Food(QVector2D initial_velocity, QPoint initial_position, qreal mass, QRect *game_size, QObject *parent)
    : QObject(parent)
    , _ball_properties(new Ball(game_size, initial_velocity, initial_position, mass, this))
    , _enabled(true)

{
    _connect_ball_property_signals();
    _ball_properties->set_velocity_ticks(30);
    _ball_properties->start_counting_velocity_ticks();
}

void Food::_connect_ball_property_signals()
{
    connect(_ball_properties, &Ball::x_changed, this, &Food::x_changed);
    connect(_ball_properties, &Ball::y_changed, this, &Food::y_changed);
}

QPoint Food::position()
{
    return _ball_properties->position();
}

int Food::x()
{
    return _ball_properties->x();
}

int Food::y()
{
    return _ball_properties->y();
}

qreal Food::mass()
{
    return _ball_properties->mass();
}


qreal Food::radius()
{
    return _ball_properties->radius();
}

bool Food::enabled()
{
    return _enabled;
}

void Food::set_enabled(bool value)
{
    _enabled = value;
    emit enabled_changed();
}

bool Food::is_disabled()
{
    return !_enabled;
}

Ball *Food::ball_properties()
{
    return _ball_properties;
}
