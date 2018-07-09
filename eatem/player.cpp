#include "player.h"
#include <QRandomGenerator>
#include <QPointer>
#include <QDebug>


// The Player constructor function
Player::Player(QRect *game_size, QObject *parent)
    : QObject(parent)
    // `_can_merg`e tracks if we can remerge a cell
    // into another cell.
    // defaults to `true`, but changes to false when
    // we request a split
    , _can_merge(true)
    , _game_size(game_size)
{
    // create a random number generator to get a random color
    QRandomGenerator random = QRandomGenerator::securelySeeded();

    // get a random color using our random number generator.
    // the hue is the only thing that changes here, the saturation and lightness
    // remain constant
    _hue = QColor::fromHsl(random.bounded(360), 255, 127);

    // A cell is the physcial part of the player, it's the actual representation on the screen
    Cell *start_cell = new Cell(_game_size, this);

    // `_cells` is a list of `Cell` objects. The `Player` class tracks and manages
    // each cell. Players can request to split cells using the spacebar.
    // If players run into a virus and they are bigger than it, it will also split them,
    // to a certain point.
    _cells.append(start_cell);

    // QML needs a `QVariantList`, so we maintain two lists
    // 1. an actual cell object list
    // 2. a `QVariantList` composed of `QVariant`s who's values are `Cell*` (`Cell` pointers)
    // Here we create our first `QVariant` who's value is a pointer to our first player Cell
    _javascript_cell_list.append(QVariant::fromValue<Cell*>(start_cell));
}

// `hue`
//     Hue getter function
QColor Player::hue()
{
    // return the hue, which is a `QColor` instance
    return _hue;
}

// `_handle_two_cell_case`
//         A private function that...
void Player::_handle_two_cell_case(Cell *left, Cell *right, int mouse_x, int mouse_y)
{
    bool cells_touching = left->is_object_touching(right->x(), right->y(), right->radius());
    if (!cells_touching)
    {
        left->request_coordinates(mouse_x, mouse_y);
        right->request_coordinates(mouse_x, mouse_y);
    }
    else if (_can_merge && cells_touching)
    {
        combine_cells(left, right);
    }
    else if (_can_merge)
    {
        left->request_coordinates(mouse_x, mouse_y);
        right->request_coordinates(mouse_x, mouse_y);
    }
    else {
        left->request_coordinates(mouse_x, mouse_y, right);
        right->request_coordinates(mouse_x, mouse_y, left);
    }
}

int Player::calc_x()
{
    if (_cells.length() == 1)
        return _cells[0]->x();

    // FIXME: need to calculate the center of player when they have more than one cell
}

int Player::calc_y()
{
    if (_cells.length() == 1)
        return _cells[0]->y();
    // FIXME: need to calculate the center of player when they have more than one cell
}

qreal Player::calc_zoom_factor()
{
    if (_cells.length() == 1)
    {
        float value = 30./_cells[0]->radius();
        if (value > 0.9)
            return 1.;
        else if (value > 0.8)
            return 0.9;
        else if (value > 0.7)
            return 0.8;
        else if (value > 0.6)
            return 0.7;
        else
            return 0.6;
    }
}

// `combine_cells`
//     A protected function that...
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


// `request_coordinates`
//     a `Q_INVOKABLE`
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
            cell->add_mass(food->mass());

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
            _merge_timer_id = startTimer(5000);
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
