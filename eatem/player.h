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

public:
    explicit Player(QObject *parent = nullptr);
    Q_INVOKABLE void request_coordinates(int x, int y);
    Q_INVOKABLE void request_split(int mouse_x, int mouse_y);

    QColor hue();

    void handle_touch(Food *food);
    void handle_touch(Virus *virus);
    void handle_touch(Player *other_player);
    bool _touching_helper(int other_x, int other_y);
    QVariantList get_cells();

signals:
    void cells_updated();

protected:
    void combine_cells(Cell* left, Cell* right);
    void explode_cell_from_virus(Cell* cell, Virus* virus);
    void timerEvent(QTimerEvent *event);

private:
    void validate_coordinates();
    void _handle_two_cell_case(Cell* left, Cell* right, int x, int y);

    QColor _hue;
    CellList _cells;
    QVariantList _javascript_cell_list;
    bool _can_merge;
    QMultiHash<Cell*, Cell*> _cell_touches;
    int _merge_timer_id;
};

#endif // PLAYER_H
