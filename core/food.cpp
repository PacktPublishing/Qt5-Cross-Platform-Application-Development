#include "food.h"

#include <QRect>
#include <QtMath>
#include <QTimerEvent>
#include <QRandomGenerator>
#include <QDebug>

Food::Food(QRect *game_size, QObject *parent)
    : QObject(parent)
    , _game_size(game_size)
    , _mass(Food::initial_mass)
    , _hue(QColor::fromHsl(QRandomGenerator::securelySeeded().bounded(360), 255, 127))
{
    generate();
}

Food::Food(QVector2D initial_velocity, QPoint initial_position, qreal mass, QRect *game_size, QColor hue, QObject *parent)
    : QObject(parent)
    , _game_size(game_size)
    , _position(initial_position)
    , _mass(mass)
    , _initial_velocity(initial_velocity)
    , _velocity_ticker(30)
    , _timer_id(startTimer(10))
    , _hue(hue)
    , _enabled(true)

{
}

void Food::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == _timer_id)
    {
        _position.setX(_position.x() + _initial_velocity.x());
        _position.setY(_position.y() + _initial_velocity.y());
        if (_velocity_ticker == 0)
            killTimer(_timer_id);

        _velocity_ticker--;
        validate_coordinates();
    }
}

void Food::validate_coordinates()
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

QPoint Food::position()
{
    return _position;
}

void Food::generate()
{
    // securely seeded ensures randomness
    QRandomGenerator random = QRandomGenerator::securelySeeded();
    _position.setX(random.bounded(_game_size->width()));
    _position.setY(random.bounded(_game_size->height()));

    _enabled = true;
}

int Food::x()
{
    return _position.x();
}

int Food::y()
{
    return _position.y();
}

qreal Food::mass()
{
    return _mass;
}


qreal Food::radius()
{
    return qSqrt(_mass/M_PI);
}

QColor Food::hue()
{
    return _hue;
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

QVector2D Food::intial_velocity()
{
    return _initial_velocity;
}
