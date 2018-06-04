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
    explicit Virus(QObject *parent = nullptr);

    int get_x();
    int get_y();
    int get_radius();


private:
    int _x;
    int _y;
    int _radius;
};

#endif // VIRUS_H
