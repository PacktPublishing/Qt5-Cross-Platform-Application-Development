#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QVariant>
#include <QPoint>
#include <QString>

class Ball;
class Cell;
class Food;
class Virus;
class GameInterface;


class Player : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int x READ calc_x NOTIFY x_changed)
    Q_PROPERTY(int y READ calc_y NOTIFY y_changed)
    Q_PROPERTY(QVariantList cells READ cells NOTIFY cells_changed)
    Q_PROPERTY(qreal zoom_factor READ calc_zoom_factor NOTIFY zoom_changed)

public:
    explicit Player(QString authentication,
                    QRect *game_size,
                    GameInterface *game_interface = nullptr,
                    QObject *parent = nullptr);

    Q_INVOKABLE void request_coordinates(int x, int y, QString authentication);
    Q_INVOKABLE void request_split(int mouse_x, int mouse_y, QString authentication);
    Q_INVOKABLE void request_fire_food(int mouse_x, int mouse_y, QString authentication);

    QVariantList cells();

    typedef QList<Cell*> CellList;

    CellList internal_cell_list();

    int calc_x();
    int calc_y();
    qreal calc_zoom_factor();
    QString authentication();

    void handle_touch(Player *other_player);
    void handle_touch(Virus *virus);

    void move();

signals:
    void x_changed();
    void y_changed();
    void cells_changed();
    void zoom_changed();
    /*
    void new_cell(Cell *cell);
    void remove_cell(Cell *cell);
    */

protected:
    void combine_cells(Cell* left, Cell* right);
    void explode_cell_from_virus(Cell* cell, Virus* virus);
    bool _handle_multiple_cells(QPoint mouse_position);

private:
    void validate_coordinates();
    void _handle_two_cell_case(Cell* left, Cell* right, QPoint mouse_position);

    // --------------------------------------------------------
    // Here's a list of all our private variables for the class
    // --------------------------------------------------------

    bool _can_merge;
    int _merge_tick_countdown;

    QVariantList _cells;

    QRect *_game_size;
    QPoint _average_position;

    GameInterface *_game_interface;
    QString _authentication;
};

#endif // PLAYER_H
