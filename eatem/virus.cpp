#include "virus.h"
#include <QtMath>
#include <QRandomGenerator>
#include "food.h"

Virus::Virus(QRect *game_size, QObject *parent)
    : QObject(parent)
    , _game_size(game_size)
    , _mass(Virus::_initial_mass)
{
    QRandomGenerator random = QRandomGenerator::securelySeeded();
    _position.setX(random.bounded(_game_size->width()));
    _position.setY(random.bounded(_game_size->height()));
}

QPoint Virus::position()
{
    return _position;
}

void Virus::handle_touch(Food *food)
{
    int radiuses = qPow(radius() + food->radius(), 2);
    int diff_x = qPow(_position.x() - food->x(), 2);
    int diff_y = qPow(_position.y() - food->y(), 2);

    bool user_fired = false;

    if (radiuses > diff_x + diff_y){
        food->set_enabled(false);
        if (food->mass() > Food::initial_mass)
            user_fired = true;

        add_mass(food->mass());
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
    if (_mass > Virus::_split_mass)
    {

    }
}
