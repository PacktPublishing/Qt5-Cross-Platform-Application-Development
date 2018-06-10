#ifndef CELL_H
#define CELL_H

#include <QObject>
#include <QPointer>

class Cell : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int x READ get_x)
    Q_PROPERTY(int y READ get_y)
    Q_PROPERTY(int radius READ get_radius)
    Q_PROPERTY(int mass READ get_mass)
public:
    explicit Cell(QObject *parent, int intial_velocity=0);
    int get_x();
    int get_y();
    int get_radius();
    int get_mass();
    void add_mass(int amount);
    void request_coordinates(int x, int y);
    bool is_object_touching(int object_x, int object_y);
    bool is_object_touching(int object_x, int object_y, int object_radius);
    QPointer<Cell> request_split(int mouse_x, int mouse_y, QObject *new_cell_parent);
    qreal get_radians(int x, int y);

    bool friendly_cell_touching();
    void set_sister_cell_touching(bool touching);

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
    bool _friendly_cell_touching;
};

#endif // CELL_H
