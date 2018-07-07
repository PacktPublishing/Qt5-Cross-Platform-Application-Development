#include "virus.h"

Virus::Virus(QObject *parent)
    : QObject(parent)
    , _radius(10)
{
    QRandomGenerator random = QRandomGenerator();
    _position.setX(random.bounded(500));
    _position.setY(random.bounded(500));
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
    return _radius;
}

int Virus::mass()
{
    return _mass;
}
