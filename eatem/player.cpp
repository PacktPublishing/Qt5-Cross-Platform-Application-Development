#include "player.h"
#include <QRandomGenerator>
#include <QPointer>
#include <QDebug>


Player::Player(QObject *parent) : QObject(parent)
{
    QRandomGenerator random = QRandomGenerator::securelySeeded();
    _hue = QColor::fromHsl(random.bounded(360), 255, 127);
    Cell *start_cell = new Cell(this);
    _cells.append(start_cell);
    _this_thing.append(QVariant::fromValue<Cell*>(start_cell));
}

QColor Player::get_hue()
{
    return _hue;
}

void Player::request_coordinates(int x, int y)
{
    for (Cell *cell : _cells)
    {
        cell->request_coordinates(x, y);
    }
}


void Player::handle_touch(Food *food)
{
    for (Cell *cell : _cells)
    {
        if (cell->is_object_touching(food->get_x(), food->get_y()))
        {
            // disable the food
            food->set_enabled(false);

            // add the mass
            cell->add_mass(1);

            // we're done here!
            break;
        }
    }
}

void Player::handle_touch(Player *other_player)
{
    for(Cell *cell : _cells)
    {
        // FIXME: this will be more complicated because
        // the radius will come into larger effect than points.
    }

}

QVariantList Player::get_cells()
{
    qDebug() << "Here" << _this_thing.length();
    return _this_thing;
}

void Player::handle_touch(Virus *virus)
{

    for(Cell *cell : _cells)
    {
        // FIXME: this will be more complicated because
        // the radius will come into larger effect than points.
    }
}

void Player::draw_cells(QObject *context)
{
    // FIXME: Not sure if this logic is correct.
    QPointer<QObject> my_pointer(context);
    if (my_pointer.isNull())
        return;

    context->setProperty("fillStyle", _hue);
    for(Cell *cell : _cells)
    {
        QMetaObject::invokeMethod(context, "beginPath");
        QMetaObject::invokeMethod(context,
                                  "arc",
                                  Q_ARG(int, cell->get_x()),
                                  Q_ARG(int, cell->get_y()),
                                  Q_ARG(int, cell->get_radius()),
                                  0,
                                  Q_ARG(qreal, 2*M_PI));

        // TODO: Make sure this is valid?
        QMetaObject::invokeMethod(context, "endPath");
    }
}
