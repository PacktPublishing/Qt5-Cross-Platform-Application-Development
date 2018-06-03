#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QtMath>
#include <QVariant>
#include <QDebug>

class Player : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int x READ get_x)
    Q_PROPERTY(int y READ get_y)
    Q_PROPERTY(int radius READ get_radius)
    Q_PROPERTY(int mass READ get_mass)

public:
    explicit Player(QObject *parent = nullptr);
    int get_x() {return _x;}
    int get_y() {return _y;}
    int get_radius();
    int get_mass() {return _mass;}

    Q_INVOKABLE void request_coordinates(int x, int y);



private:
    void validate_coordinates();
    int _x;
    int _y;
    int _mass;
};

#endif // PLAYER_H
