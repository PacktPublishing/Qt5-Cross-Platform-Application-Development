#ifndef VIRUS_H
#define VIRUS_H

#include <QObject>
#include <QPoint>
#include <QRandomGenerator>
#include <QRect>
#include "food.h"

class Virus : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int x READ x)
    Q_PROPERTY(int y READ y)
    Q_PROPERTY(int radius READ radius)

public:
    explicit Virus(QRect *game_size, QObject *parent = nullptr);
    static constexpr qreal _initial_mass = 22167;
    static constexpr qreal _radius = 84;

    int x();
    int y();
    int radius();
    qreal mass();
    void add_mass(qreal mass);
    void handle_touch(Food *food);

private:
    QPoint _position;
    QRect *_game_size;
    qreal _mass;
};

#endif // VIRUS_H
