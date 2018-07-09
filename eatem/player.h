#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QtMath>
#include <QVariant>
#include <QColor>
#include <QDebug>

#include "cell.h"
#include "food.h"
#include "virus.h"

Q_DECLARE_METATYPE(Cell *)
typedef QList<Cell*> CellList;


class Player : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QColor hue READ hue)
    Q_PROPERTY(QVariantList cells READ get_cells NOTIFY cells_updated)
    Q_PROPERTY(int x READ calc_x)
    Q_PROPERTY(int y READ calc_y)
    Q_PROPERTY(qreal zoom_factor READ calc_zoom_factor)


public:
    explicit Player(QRect *game_size, QObject *parent = nullptr);
    // NOTE: these are invokeables and not slots because it's easier to pass multiple functions
    // into a invokable?
    Q_INVOKABLE void request_coordinates(int x, int y);
    Q_INVOKABLE void request_split(int mouse_x, int mouse_y);
    Q_INVOKABLE void request_fire_food(int mouse_x, int mouse_y, QString authentication);

    QColor hue();

    void handle_touch(Food *food);
    void handle_touch(Virus *virus);
    void handle_touch(Player *other_player);
    bool _touching_helper(int other_x, int other_y);
    QVariantList get_cells();

    int calc_x();
    int calc_y();
    qreal calc_zoom_factor();

signals:
    void cells_updated();

protected:
    void combine_cells(Cell* left, Cell* right);
    void explode_cell_from_virus(Cell* cell, Virus* virus);
    void timerEvent(QTimerEvent *event);

private:
    void validate_coordinates();
    void _handle_two_cell_case(Cell* left, Cell* right, int x, int mouse_y);

    // --------------------------------------------------------
    // Here's a list of all our private variables for the class
    // --------------------------------------------------------

    // `_hue`
    //      The color of the cells/this player
    QColor _hue;

    // `_cells`
    //      a list of all the cells
    CellList _cells;
    // The type, `CellList` is `QList` of `Cell*` ( QList<Cell*> )
    // and is defined on line 15 of this document


    // `_javascript_cell_list`
    //     is a list of all the cells
    QVariantList _javascript_cell_list;
    // QML needs a `QVariantList` which acts as list in JavaScript
    // `QVariantList` ==  `QList<QVariant>`
    // Because this class manages the list of `Cell` pointers, it
    // also manages this list.


    // `_can_merge`
    //     tracks if a cell can combine again if the player has multiple cells
    bool _can_merge;

    // `_cell_touches`
    //     A hash of all the cells that touch each other
    QMultiHash<Cell*, Cell*> _cell_touches;


    int _merge_timer_id;
    QRect *_game_size;
};

#endif // PLAYER_H
