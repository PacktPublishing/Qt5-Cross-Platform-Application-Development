#ifndef CELL_H
#define CELL_H

#include <QObject>

class Cell : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int x READ get_x)
    Q_PROPERTY(int y READ get_y)
    Q_PROPERTY(int radius READ get_radius)
    Q_PROPERTY(int mass READ get_mass)
public:
    explicit Cell(QObject *parent = nullptr);
    int get_x();
    int get_y();
    int get_radius();
    int get_mass();
    void add_mass(int amount);
    void request_coordinates(int x, int y);

private:
    void validate_coordinates();

    int _x;
    int _y;
    int _mass;
};

#endif // CELL_H
