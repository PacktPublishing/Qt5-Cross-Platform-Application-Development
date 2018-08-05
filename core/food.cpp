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
}

Food::Food(QVector2D initial_velocity, QPoint initial_position, qreal mass, QRect *game_size, QObject *parent)
    : QObject(parent)
    , _ball_properties(new Ball(game_size, initial_velocity, 30, initial_position, mass, hue, this))
    , _enabled(true)

{
}

QPoint Food::position()
{
    return _ball_properties->position();
}

int Food::x()
{
    return _ball_properties->x()
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
