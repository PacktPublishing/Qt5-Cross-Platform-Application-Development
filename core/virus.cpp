#include "virus.h"

#include <QtMath>
#include <QDebug>

#include "food.h"
#include "gameinterface.h"
#include "ball.h"

Virus::Virus(QRect *game_size, GameInterface *game_interface, QObject *parent)
    : QObject(parent)
    , _ball_properties(new Ball(game_size, Virus::_initial_mass, this))
    , _game_interface(game_interface)
{
    _connect_ball_property_signals();
    _ball_properties->set_coordinates_random();
}

Virus::Virus(Ball *ball_properties, GameInterface *game_interface, QObject *parent)
    : QObject(parent)
    , _ball_properties(ball_properties)
    , _game_interface(game_interface)
{
    _connect_ball_property_signals();
    _ball_properties->set_velocity_ticks(30);
    _ball_properties->setParent(this);
}

void Virus::move()
{
    _ball_properties->move();
}

QPoint Virus::position()
{
    return _ball_properties->position();
}

Ball* Virus::ball_properties()
{
    return _ball_properties;
}

bool Virus::is_touching(Ball *other)
{
    return _ball_properties->is_touching(other);
}

void Virus::_connect_ball_property_signals()
{
    connect(_ball_properties, &Ball::x_changed, this, &Virus::x_changed);
    connect(_ball_properties, &Ball::y_changed, this, &Virus::y_changed);
    connect(_ball_properties, &Ball::radius_changed, this, &Virus::radius_changed);
}

void Virus::eat_food(Food *food)
{
    _ball_properties->add_mass(food->mass());

    if (_ball_properties->mass() > Virus::_split_mass && _game_interface)
    {
        QVector2D velocity = food->ball_properties()->initial_velocity();
        velocity *= 10;

        Ball *new_ball = new Ball(*_ball_properties);
        new_ball->set_initial_velocity(velocity);

        Virus *virus = new Virus(new_ball, _game_interface);
        _game_interface->track_new_virus(virus);
        _ball_properties->set_mass(Virus::_initial_mass);
    }
}

int Virus::x()
{
    return _ball_properties->x();
}

int Virus::y()
{
    return _ball_properties->y();
}

int Virus::radius()
{
    return _ball_properties->radius();
}

qreal Virus::mass()
{
    return _ball_properties->mass();
}

void Virus::add_mass(qreal mass)
{
    return _ball_properties->add_mass(mass);
}
