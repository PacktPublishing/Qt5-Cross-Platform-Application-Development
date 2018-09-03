#include "food.h"

#include <QRect>
#include <QtMath>
#include <QRandomGenerator>
#include <QDebug>
#include "ball.h"

Food::Food(QRect *game_size, QObject *parent)
    : QObject(parent)
    , _ball_properties(new Ball(game_size, Food::initial_mass, this))
{
    _ball_properties->set_coordinates_random();
    _connect_ball_property_signals();
}

Food::Food(Ball* ball_properties, QObject *parent)
    : QObject(parent)
    , _ball_properties(ball_properties)

{
    _connect_ball_property_signals();
    _ball_properties->set_velocity_ticks(30);
    _ball_properties->setParent(this);
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

Ball *Food::ball_properties()
{
    return _ball_properties;
}

void Food::move()
{
    _ball_properties->move();
}
