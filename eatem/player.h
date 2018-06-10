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
    Q_PROPERTY(QColor hue READ get_hue)
    Q_PROPERTY(QVariantList cells READ get_cells)

public:
    explicit Player(QObject *parent = nullptr);
    Q_INVOKABLE void request_coordinates(int x, int y);
    Q_INVOKABLE void request_split(int mouse_x, int mouse_y);

    QColor get_hue();

    void handle_touch(Food *food);
    void handle_touch(Virus *virus);
    void handle_touch(Player *other_player);
    bool _touching_helper(int other_x, int other_y);
    QVariantList get_cells();

private:
    void validate_coordinates();

    QColor _hue;
    CellList _cells;
    QVariantList _this_thing;
};

#endif // PLAYER_H
