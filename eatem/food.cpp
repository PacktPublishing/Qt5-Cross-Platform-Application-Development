#include "food.h"

Food::Food(QRect *game_size, QObject *parent)
    : QObject(parent)
    , _game_size(game_size)
{
    generate();
}

Food::Food(QVector2D intial_velocity, QPoint initial_position, QRect *game_size, QObject *parent)
    : QObject(parent)
    , _game_size(game_size)
    , _position(initial_position)
{
    // FIXME: add in velocity handeling
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
