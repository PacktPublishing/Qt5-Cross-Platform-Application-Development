#ifndef FOOD_H
#define FOOD_H

#include <QObject>
#include <QPoint>
#include <QRect>
#include <QColor>
#include <QVector2D>
#include <QRandomGenerator>


class Food : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int x READ x)
    Q_PROPERTY(int y READ y)
    Q_PROPERTY(int radius READ radius)
    Q_PROPERTY(bool enabled READ enabled)
    Q_PROPERTY(QColor hue READ hue)
public:
    explicit Food(QRect *game_size, QObject *parent = nullptr);
    explicit Food(QVector2D intial_velocity, QPoint initial_position, QRect *game_size, QObject *parent = nullptr);

    void generate();

    int x();
    int y();
    qreal radius();
    qreal mass();
    QColor hue();
    bool enabled();
    QPoint position();
    void set_enabled(bool value);
    bool is_disabled();

private:
    static constexpr qreal _mass = 78.54;
    static constexpr qreal _radius = 5.0;

    QPoint _position;
    QColor _hue;
    bool _enabled;
    QRect *_game_size;
};

#endif // FOOD_H
