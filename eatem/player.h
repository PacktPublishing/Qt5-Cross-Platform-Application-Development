#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QtMath>
#include <QVariant>
#include <QColor>
#include <QDebug>
#include "cell.h"

class Food;
class Virus;
typedef CellList QList<Cell>;

class Player : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QColor hue READ get_hue)

public:
    explicit Player(QObject *parent = nullptr);
    QColor get_hue();
    Q_INVOKABLE void draw_cells(QObject *context)
    {
        context->setProperty("fillStyle", _hue);
        foreach(QVariant cell_variant, _cells)
        {
            // FIXME: might not need this to be a QVariant list??
            // if we're drawing from here.
            Cell *cell = qvariant_cast<Cell*>(cell_variant);

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

    Q_INVOKABLE void request_coordinates(int x, int y);

    bool touching(Food *food, int fudge_factor)
    {
    }

    bool touching(Virus *virus);
    bool touching(Player *other_player);

    bool _touching_helper(int other_x, int other_y)
    {
        int diff_x = x - other_x;
        int diff_y = y - other_y;
        float distance = pow((pow(diff_x, 2) + pow(diff_y, 2)), 0.5);
        return distance <= radius;
    }


private:
    void validate_coordinates();
    QColor _hue;
    CellList _cells;
};

#endif // PLAYER_H
