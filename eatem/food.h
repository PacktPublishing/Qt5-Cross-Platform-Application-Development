#ifndef FOOD_H
#define FOOD_H

#include <QObject>
#include <QRandomGenerator>

class Food : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int x READ get_x)
    Q_PROPERTY(int y READ get_y)
    Q_PROPERTY(int radius READ get_radius)
public:
    explicit Food(QObject *parent = nullptr)
    {
        QRandomGenerator random = QRandomGenerator::securelySeeded();
        _x = random.bounded(500);
        _y = random.bounded(500);
    }

    int get_x() {return _x;}
    int get_y() {return _y;}
    int get_radius() {return _radius;}

private:
    int _x;
    int _y;
    int _radius;
    QString _hue;

};

#endif // FOOD_H
