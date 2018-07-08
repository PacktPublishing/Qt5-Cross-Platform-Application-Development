#ifndef VIRUS_H
#define VIRUS_H

#include <QObject>
#include <QPoint>
#include <QRandomGenerator>

class Virus : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int x READ x)
    Q_PROPERTY(int y READ y)
    Q_PROPERTY(int radius READ radius)

public:
    explicit Virus(QRect *game_size, QObject *parent = nullptr);

    int x();
    int y();
    int radius();
    int mass();


private:
    QPoint _position;
    QRect *_game_size;
    // FIXME: radius is dependent on mass
    int _radius;
    int _mass;
};

#endif // VIRUS_H
