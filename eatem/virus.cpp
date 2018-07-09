#include "virus.h"
#include <QtMath>

Virus::Virus(QRect *game_size, QObject *parent)
    : QObject(parent)
    , _game_size(game_size)
    , _mass(Virus::_initial_mass)
{
    QRandomGenerator random = QRandomGenerator::securelySeeded();
    _position.setX(random.bounded(_game_size->width()));
    _position.setY(random.bounded(_game_size->height()));
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
}
