#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QVariant>
#include <QColor>
#include <QPoint>

class Cell;
class Food;
class Virus;
class QTimerEvent;
class GameInterface;

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
    explicit Player(QRect *game_size,
                    GameInterface *game_interface = nullptr,
                    QObject *parent = nullptr);

    Q_INVOKABLE void request_coordinates(int x, int y, QString authentication);
    Q_INVOKABLE void request_split(int mouse_x, int mouse_y, QString authentication);
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
    void _handle_two_cell_case(Cell* left, Cell* right, QVector2D mouse_position);

    // --------------------------------------------------------
    // Here's a list of all our private variables for the class
    // --------------------------------------------------------

    QColor _hue;
    CellList _cells;
    QVariantList _javascript_cell_list;
    bool _can_merge;
    QMultiHash<Cell*, Cell*> _cell_touches;
    int _merge_timer_id;
    QRect *_game_size;
    QPoint _average_position;
    GameInterface *_game_interface;
};

#endif // PLAYER_H
