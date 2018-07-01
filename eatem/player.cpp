#include "player.h"
#include <QRandomGenerator>
#include <QPointer>
#include <QDebug>


Player::Player(QObject *parent)
    : QObject(parent)
    , _can_merge(true)
{
    QRandomGenerator random = QRandomGenerator::securelySeeded();
    _hue = QColor::fromHsl(random.bounded(360), 255, 127);
    Cell *start_cell = new Cell(this);
    _cells.append(start_cell);
    _javascript_cell_list.append(QVariant::fromValue<Cell*>(start_cell));
}

QColor Player::hue()
{
    return _hue;
}

void Player::_handle_two_cell_case(Cell *left, Cell *right, int x, int y)
{
    bool cells_touching = left->is_object_touching(right->x(), right->y(), right->radius());
    if (!cells_touching)
    {
        left->request_coordinates(x, y);
        right->request_coordinates(x, y);
    }
    else if (_can_merge && cells_touching)
    {
        combine_cells(left, right);
    }
    else if (_can_merge)
    {
        left->request_coordinates(x, y);
        right->request_coordinates(x, y);
    }
    else {
        left->request_coordinates(x, y, right);
        right->request_coordinates(x, y, left);
    }
}

void Player::combine_cells(Cell *left, Cell *right)
{
    if (left->mass() > right->mass())
    {
        left->add_mass(right->mass());
        _cells.removeOne(right);
        // FIXME: remembering to do this not worth keeping data in two different
        // forms. Not the correct layer of abstraction
        _javascript_cell_list.removeOne(QVariant::fromValue<Cell*>(right));
        right->deleteLater();
    }
    else
    {
        right->add_mass(left->mass());
        _cells.removeOne(left);
        _javascript_cell_list.removeOne(QVariant::fromValue<Cell*>(left));
        left->deleteLater();
    }
    emit cells_updated();

}

// https://www.reddit.com/r/Agario/comments/34x2fa/game_mechanics_explained_in_depth_numbers_and/
// https://stackoverflow.com/questions/5060082/eliminating-a-direction-from-a-vector
void Player::request_coordinates(int x, int y)
{
    // Hardcode in the most common options, no cell split
    if (_cells.length() == 1)
    {
        _cells[0]->request_coordinates(x, y);
        return;
    }
    // Hardcode in second most common option, cell split once
    else if(_cells.length() == 2)
    {
        Cell* left = _cells[0];
        Cell* right = _cells[1];
        return _handle_two_cell_case(left, right, x, y);
    }

    _cell_touches.clear();

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
                    _cell_touches.insert(cell, other_cell);
        }

        QList<Cell*> all_cell_touches = _cell_touches.values(cell);
        cell->request_coordinates(x, y, all_cell_touches);
    }
}


void Player::handle_touch(Food *food)
{
    for (Cell *cell : _cells)
    {
        if (cell->is_object_touching(food->x(), food->y(), food->radius()))
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

void Player::timerEvent(QTimerEvent *event)
{
    _can_merge = true;
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
            _javascript_cell_list.append(QVariant::fromValue<Cell*>(new_cell));
            _cells.append(new_cell);
            // TODO: put a smaller nubmer in here if we have a bunch of splits
            _merge_timer_id = startTimer(2000);
            _can_merge = false;
        }
    }
}

QVariantList Player::get_cells()
{
    return _javascript_cell_list;
}

void Player::handle_touch(Virus *virus)
{
    for(Cell *cell : _cells)
    {
        // TODO: add in fudge factor to radius
        if (cell->is_object_touching(virus->x(), virus->y(), virus->radius()))
        {
            // compare mass since there's no math
            if (cell->mass() <= virus->mass())
                continue;
            // FIXME: you can eat viruses if you're big enough
            // else if ()
            else
                explode_cell_from_virus(cell, virus);
        }
    }
}

void Player::explode_cell_from_virus(Cell *cell, Virus *virus)
{

}
