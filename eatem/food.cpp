#include "food.h"
#include <QTimerEvent>
#include <QRandomGenerator>
#include <QRect>

Food::Food(QRect *game_size, QObject *parent)
    : QObject(parent)
    , _game_size(game_size)
    , _mass(Food::initial_mass)
{
    generate();
}

Food::Food(QVector2D initial_velocity, QPoint initial_position, qreal mass, QRect *game_size, QObject *parent)
    : QObject(parent)
    , _game_size(game_size)
    , _position(initial_position)
    , _mass(mass)
    , _initial_velocity(initial_velocity)
    , _velocity_ticker(10)
    , _timer_id(startTimer(500))

{
}

void Food::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == _timer_id)
    {
        if (_velocity_ticker == 0)
            killTimer(_timer_id);

        _velocity_ticker--;
    }
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

    _hue = QColor::fromHsl(random.bounded(360), 255, 127);
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
    return _radius;
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
}

bool Food::is_disabled()
{
    return !_enabled;
}
