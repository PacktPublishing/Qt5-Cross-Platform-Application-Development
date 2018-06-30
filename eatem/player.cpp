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

QColor Player::hue()
{
    return _hue;
}

// https://www.reddit.com/r/Agario/comments/34x2fa/game_mechanics_explained_in_depth_numbers_and/
// https://stackoverflow.com/questions/5060082/eliminating-a-direction-from-a-vector
void Player::request_coordinates(int x, int y)
{
    QMultiHash<Cell*, Cell*> cell_touches;
    // For every cell
    for (Cell *cell : _cells)
    {
        // Check if we're in contact with our own cells
        // by iterating through every cell again
        for (Cell *other_cell : _cells)
        {
            if (cell == other_cell)
                continue;

            if (cell->is_object_touching(other_cell->x(), other_cell->y(), other_cell->radius()))
                    cell_touches.insert(cell, other_cell);
        }

    }

    for (Cell *cell : _cells)
    {
        // check touching?
        if (cell->friendly_cell_touching())
        {
        }
        else
            cell->request_coordinates(x, y);
    }
}


void Player::handle_touch(Food *food)
{
    for (Cell *cell : _cells)
    {
        if (cell->is_object_touching(food->x(), food->y()))
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

void Player::request_split(int mouse_x, int mouse_y)
{
    for (Cell* cell : _cells)
    {
        // Create a new pointer
        QPointer<Cell> split_cell;
        // request the cells to split
        split_cell = cell->request_split(mouse_x, mouse_y, this);
        // check to see if we got a new split cell
        if (!split_cell.isNull())
        {
            // Track the new split cell if we did
            Cell* new_cell = split_cell.data();
            _this_thing.append(QVariant::fromValue<Cell*>(new_cell));
            _cells.append(new_cell);
        }
    }
}

QVariantList Player::get_cells()
{
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
