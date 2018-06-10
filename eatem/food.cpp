#include "food.h"

Food::Food(int game_width, int game_height, QObject *parent)
    : QObject(parent)
    , _game_height(game_height)
    , _game_width(game_width)
    , _radius(5)
{
    generate();
}


void Food::generate()
{
    // securely seeded ensures randomness
    QRandomGenerator random = QRandomGenerator::securelySeeded();
    _x = random.bounded(_game_width);
    _y = random.bounded(_game_height);
    _hue = QColor::fromHsl(random.bounded(360), 255, 127);
    _enabled = true;
}

