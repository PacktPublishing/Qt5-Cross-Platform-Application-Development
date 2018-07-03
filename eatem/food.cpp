#include "food.h"

Food::Food(QRect game_size, QObject *parent)
    : QObject(parent)
    , _game_size(game_size)
    , _radius(5)
{
    generate();
}


void Food::generate()
{
    // securely seeded ensures randomness
    QRandomGenerator random = QRandomGenerator::securelySeeded();
    _position.setX(random.bounded(_game_size.width()));
    _position.setY(random.bounded(_game_size.height()));

    _hue = QColor::fromHsl(random.bounded(360), 255, 127);
    _enabled = true;
}

