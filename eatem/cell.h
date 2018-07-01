#ifndef CELL_H
#define CELL_H

#include <QObject>
#include <QPointer>

class Player;

class Cell : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int x READ x)
    Q_PROPERTY(int y READ y)
    Q_PROPERTY(int radius READ radius)
    Q_PROPERTY(int mass READ mass)
public:
    explicit Cell(QObject *parent, int intial_velocity=0);
    void add_mass(int amount);
    void request_coordinates(int x, int y);
    void request_coordinates(int x, int y, QList<Cell*> touching_cells);
    void request_coordinates(int x, int y, Cell* touching_cells);
    bool is_object_touching(int object_x, int object_y, int object_radius);
    QPointer<Cell> request_split(int mouse_x, int mouse_y, Player *owning_player);
    qreal calc_radians(int x, int y);

    // Getters
    int x();
    int y();
    int radius();
    int mass();

protected:
    void timerEvent(QTimerEvent *event);

private:
    void validate_coordinates();

    int _x;
    int _y;
    // NOTE: used when firing
    int _velocity;
    int _mass;
    int _timer_id;
    // NOTE: This should probably be static
    int _initial_mass;
};

#endif // CELL_H
