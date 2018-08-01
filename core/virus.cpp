#include "virus.h"

#include <QtMath>
#include <QRandomGenerator>
#include <QDebug>

#include "food.h"
#include "gameinterface.h"

Virus::Virus(QRect *game_size, GameInterface *game_interface, QObject *parent)
    : QObject(parent)
    , _game_size(game_size)
    , _mass(Virus::_initial_mass)
    , _game_interface(game_interface)
{
    QRandomGenerator random = QRandomGenerator::securelySeeded();
    int two_radius = radius() * 2;
    _position.setX(random.bounded(two_radius, _game_size->width()- two_radius));
    _position.setY(random.bounded(two_radius, _game_size->height()- two_radius));
}

Virus::Virus(QPoint position, QVector2D initial_velocity, QRect *game_size, GameInterface *game_interface, QObject *parent)
    : QObject(parent)
    , _game_size(game_size)
    , _mass(Virus::_initial_mass)
    , _game_interface(game_interface)
    , _initial_velocity(initial_velocity)
    , _timer_id(startTimer(10))
    , _velocity_ticker(40)
    , _position(position)
{
}

QPoint Virus::position()
{
    return _position;
}

void Virus::validate_coordinates()
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

    emit x_change();
    emit y_change();
}

void Virus::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == _timer_id)
    {
        qDebug() << _position << _initial_velocity;
        _position.setX(_position.x() + _initial_velocity.x());
        _position.setY(_position.y() + _initial_velocity.y());
        if (_velocity_ticker == 0)
            killTimer(_timer_id);

        _velocity_ticker--;
        validate_coordinates();
    }
}

void Virus::handle_touch(Food *food)
{
    int radiuses = qPow(radius() + food->radius(), 2);
    int diff_x = qPow(_position.x() - food->x(), 2);
    int diff_y = qPow(_position.y() - food->y(), 2);

    // If the virus and food are not touching, return
    if (!(radiuses > diff_x + diff_y))
        return;

    add_mass(food->mass());
    food->set_enabled(false);

    if (_mass > Virus::_split_mass && _game_interface)
    {
        QVector2D velocity = food->intial_velocity().normalized();
        velocity *= 10;

        Virus *virus = new Virus(_position, velocity, _game_size, _game_interface);
        _game_interface->track_new_virus(virus);
        _mass = Virus::_initial_mass;
    }
}

int Virus::x()
{
    return _position.x();
}

int Virus::y()
{
    return _position.y();
}

int Virus::radius()
{
    return qSqrt(_mass/M_PI);
}

qreal Virus::mass()
{
    return _mass;
}

void Virus::add_mass(qreal mass)
{
    _mass += mass;
    emit radius_change();
}
