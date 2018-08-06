#include "virus.h"

#include <QtMath>
#include <QDebug>

#include "food.h"
#include "gameinterface.h"
#include "ball.h"

Virus::Virus(QRect *game_size, GameInterface *game_interface, QObject *parent)
    : QObject(parent)
    , _ball_properties(new Ball(game_size, Virus::_initial_mass))
    , _game_interface(game_interface)
{
    _connect_ball_property_signals();
}

Virus::Virus(Ball *ball_properties, GameInterface *game_interface, QObject *parent)
    : QObject(parent)
    , _ball_properties(ball_properties)
    , _game_interface(game_interface)
{
    _connect_ball_property_signals();
    _ball_properties->set_velocity_ticks(30);
    _ball_properties->start_counting_velocity_ticks();
}

QPoint Virus::position()
{
    return _ball_properties->position();
}

void Virus::_connect_ball_property_signals()
{
    connect(_ball_properties, &Ball::x_changed, this, &Virus::x_changed);
    connect(_ball_properties, &Ball::y_changed, this, &Virus::y_changed);
    connect(_ball_properties, &Ball::radius_changed, this, &Virus::radius_changed);
}

void Virus::handle_touch(Food *food)
{
    int radiuses = qPow(radius() + food->radius(), 2);
    int diff_x = qPow(_ball_properties->x() - food->x(), 2);
    int diff_y = qPow(_ball_properties->y() - food->y(), 2);

    // If the virus and food are not touching, return
    if (!(radiuses > diff_x + diff_y))
        return;

    _ball_properties->add_mass(food->mass());
    food->set_enabled(false);

    if (_ball_properties->mass() > Virus::_split_mass && _game_interface)
    {
        QVector2D velocity = food->intial_velocity().normalized();
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
