#include "player.h"
#include <QRandomGenerator>


Player::Player(QObject *parent) : QObject(parent)
{
    QRandomGenerator random = QRandomGenerator();
    _hue = QColor::fromHsl(random.bounded(360), 255, 127);
}

QColor Player::get_hue()
{
    return _hue;
}

void Player::request_coordinates(int x, int y)
{
    for (QVariant cell_variant : _cells)
    {
        Cell *cell = cell_variant.type<Cell *>();
        cell->request_coordinates(x, y);
    }
}
