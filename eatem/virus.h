#ifndef VIRUS_H
#define VIRUS_H

#include <QObject>
#include <QRandomGenerator>

class Virus : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int x READ get_x)
    Q_PROPERTY(int y READ get_y)
    Q_PROPERTY(int radius READ get_radius)

public:
    explicit Virus(QObject *parent = nullptr) :
        _radius(10)
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
};

#endif // VIRUS_H
