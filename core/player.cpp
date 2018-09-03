#include "player.h"

#include <QDebug>
#include <QtMath>
#include <QPointer>
#include <QRandomGenerator>

#include "gameinterface.h"
#include "cell.h"
#include "food.h"
#include "virus.h"
#include "ball.h"

Q_DECLARE_METATYPE(Cell *)


// The Player constructor function
Player::Player(QString authentication, QRect *game_size, GameInterface *game_interface, QObject *parent)
    : QObject(parent)
    , _can_merge(true)
    , _merge_tick_countdown(-1)
    // `_can_merge` tracks if we can remerge a cell
    // into another cell.
    // defaults to `true`, but changes to false when
    // we request a split
    , _game_size(game_size)
    , _game_interface(game_interface)
    , _authentication(authentication)
{
    // A cell is the physcial part of the player, it's the actual representation on the screen
    Cell *start_cell = new Cell(_game_size, this);

    // QML needs a `QVariantList`, so we maintain two lists
    // 1. an actual cell object list
    // 2. a `QVariantList` composed of `QVariant`s who's values are `Cell*` (`Cell` pointers)
    // Here we create our first `QVariant` who's value is a pointer to our first player Cell
    _cells.append(QVariant::fromValue<Cell*>(start_cell));
}

// `_handle_two_cell_case`
//         A private function that...
void Player::_handle_two_cell_case(Cell *left, Cell *right, QPoint mouse_position)
{
    bool cells_touching = left->is_touching(right->ball_properties());
    // bool overlaped_enough
    if (!cells_touching)
    {
        left->request_coordinates(mouse_position);
        right->request_coordinates(mouse_position);
    }
    else if (_can_merge && cells_touching)
    {
        combine_cells(left, right);
    }
    else if (_can_merge)
    {
        left->request_coordinates(mouse_position);
        right->request_coordinates(mouse_position);
    }
    else {
        left->request_coordinates(mouse_position, right->ball_properties());
        right->request_coordinates(mouse_position, left->ball_properties());
    }
}

int Player::calc_x()
{
    if (_cells.length() == 1)
        return _cells[0].value<Cell *>()->x();

    int total_x = 0;
    int total_y = 0;
    for (QVariant cell_variant : _cells)
    {
        Cell *cell = cell_variant.value<Cell *>();

        total_x += cell->x();
        total_y += cell->y();
    }

    // average x
    int average_x = total_x / _cells.length();
    int average_y = total_y / _cells.length();
    _average_position.setX(average_x);
    _average_position.setY(average_y);
    return average_x;
}

int Player::calc_y()
{
    if (_cells.length() == 1)
        return _cells[0].value<Cell *>()->y();

    return _average_position.y();
}

qreal Player::calc_zoom_factor()
{
    float value;
    if (_cells.length() == 1){
        float value = 30./_cells[0].value<Cell *>()->radius();
    }
    else{
        qreal total_mass = 0;
        for (QVariant cell_variant : _cells){
            Cell *cell = cell_variant.value<Cell *>();
            total_mass += cell->mass();
        }
        // TODO: validate if this makes sense
        value = Cell::initial_mass / total_mass;
    }

    if (value > 0.8)
        return 1.;
    else if (value > 0.7)
        return .95;
    else if (value > 0.6)
        return .9;
    else if (value > 0.5)
        return .85;
    else
        return .8;
}

QString Player::authentication()
{
    return _authentication;
}

// `combine_cells`
//     A protected function that...
void Player::combine_cells(Cell *left, Cell *right)
{
    if (left->mass() > right->mass())
    {
        left->add_mass(right->mass());
        _cells.removeOne(QVariant::fromValue<Cell *>(right));
        right->deleteLater();
    }
    else
    {
        right->add_mass(left->mass());
        _cells.removeOne(QVariant::fromValue<Cell *>(left));
        left->deleteLater();
    }

    emit cells_changed();
}


// `request_coordinates`
//     a `Q_INVOKABLE`
// https://www.reddit.com/r/Agario/comments/34x2fa/game_mechanics_explained_in_depth_numbers_and/
// https://stackoverflow.com/questions/5060082/eliminating-a-direction-from-a-vector
void Player::request_coordinates(int x, int y, QString authentication)
{
    if (authentication != _authentication)
        return;

    QPoint mouse_position(x, y);
    // Hardcode in the most common options, no cell split
    if (_cells.length() == 1)
    {
        _cells[0].value<Cell *>()->request_coordinates(mouse_position);
        emit x_changed();
        emit y_changed();
        return;
    }

    // Hardcode in second most common option, cell split once
    else if(_cells.length() == 2)
    {
        Cell* left = _cells[0].value<Cell *>();
        Cell* right = _cells[1].value<Cell *>();
        _handle_two_cell_case(left, right, mouse_position);
        emit x_changed();
        emit y_changed();
        return;
    }

    bool cell_deleted = _handle_multiple_cells(mouse_position);

    if (cell_deleted)
        _handle_multiple_cells(mouse_position);


    emit x_changed();
    emit y_changed();
}

void Player::handle_touch(Player *other_player)
{
    for(QVariant cell_variant : _cells)
    {
        Cell *cell = cell_variant.value<Cell *>();
        for (QVariant other_cell_variant : other_player->cells())
        {
            Cell *other_cell = other_cell_variant.value<Cell *>();
            if (cell->is_touching(other_cell->ball_properties()))
            {

            }

        }
    }

}

void Player::request_split(int mouse_x, int mouse_y, QString authentication)
{
    if (authentication != _authentication)
        return;

    QPoint mouse_position(mouse_x, mouse_y);
    for (QVariant cell_variant : _cells)
    {
        Cell *cell = cell_variant.value<Cell *>();
        // Create a new pointer
        QPointer<Cell> split_cell;
        // request the cells to split
        split_cell = cell->request_split(mouse_position);
        // check to see if we got a new split cell
        if (!split_cell.isNull())
        {
            // Track the new split cell if we did
            Cell* cell_data = split_cell.data();
            _cells.append(QVariant::fromValue<Cell*>(cell_data));
            // TODO: put a smaller number in here if we have a bunch of splits
            _merge_tick_countdown = 100 / _cells.length();
            _can_merge = false;
            // emit new_cell(cell_data);
            emit cells_changed();
        }
    }
}

QVariantList Player::cells()
{
    return _cells;
}

void Player::handle_touch(Virus *virus)
{
    for(QVariant cell_variant : _cells)
    {
        Cell *cell = cell_variant.value<Cell *>();

        // TODO: add in fudge factor to radius
        if (cell->is_touching(virus->ball_properties()))
        {
            // compare mass since there's no math
            if (cell->mass() <= virus->mass())
                continue;
            else
                explode_cell_from_virus(cell, virus);
        }
    }
}

void Player::move()
{
    if (_merge_tick_countdown < 0 && !_can_merge)
        _can_merge = true;
    else if(_merge_tick_countdown >= 0)
        _merge_tick_countdown --;
}

void Player::explode_cell_from_virus(Cell *cell, Virus *virus)
{
    if (!_game_interface)
        return;

    int number_new_cells = static_cast<int>(cell->mass()) % static_cast<int>(Cell::initial_mass);
    if (number_new_cells + _cells.length() > 16)
        number_new_cells = 16 - _cells.length();

    _can_merge = false;
    cell->set_mass(Cell::initial_mass);
    // two radians
    qreal delta = 6.283 / number_new_cells;

    for (int i=0; i < number_new_cells; i++)
    {
        QVector2D velocity(qSin(delta * i), qCos(delta * i));
        velocity *= 10;
        Ball *ball_properties = new Ball(*cell->ball_properties());
        ball_properties->set_initial_velocity(velocity);
        ball_properties->set_velocity_ticks(30);
        Cell *new_cell = new Cell(ball_properties, this);
        _cells.append(QVariant::fromValue<Cell *>(new_cell));
    }

    _game_interface->remove_virus_from_game(virus);
}

bool Player::_handle_multiple_cells(QPoint mouse_position)
{
    QMultiHash<Cell*, Cell*> cell_touches;
    Cell *deleted_cell;

    // For every cell
    for (QVariant cell_variant : _cells)
    {
        Cell *cell = cell_variant.value<Cell *>();

        // Check if we're in contact with our own cells
        // by iterating through every cell again
        for (QVariant other_cell_variant : _cells)
        {
            if (cell_variant == other_cell_variant)
                continue;
            Cell *other_cell = other_cell_variant.value<Cell *>();

            if (cell->is_touching(other_cell->ball_properties()))
                    cell_touches.insert(cell, other_cell);
        }

        QList<Cell *> all_cell_touches = cell_touches.values(cell);
        all_cell_touches.removeAll(deleted_cell);

        // FIXME: Grab from last?
        if (_can_merge && !all_cell_touches.empty())
        {
            deleted_cell = all_cell_touches.first();
            combine_cells(cell, deleted_cell);
            // 50 ms is the game step interval
            _merge_tick_countdown = 100;
            _can_merge = false;
            return true;
        }
        else
            cell->request_coordinates(mouse_position, all_cell_touches);
    }
    return false;

}

void Player::request_fire_food(int mouse_x, int mouse_y, QString authentication)
{
    if (authentication != _authentication)
        return;

    QPoint mouse_position(mouse_x, mouse_y);

    for (QVariant cell_variant : _cells)
    {
        Cell *cell = cell_variant.value<Cell *>();
        // Create a new pointer
        QPointer<Food> new_food;
        // request the cells to split
        new_food = cell->request_fire_food(mouse_position);
        // check to see if we got a new split cell
        if (!new_food.isNull() && _game_interface)
            _game_interface->track_food_fired_by_players(new_food);
    }
}
