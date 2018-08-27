#ifndef FOOD_H
#define FOOD_H

#include <QObject>
#include <QPoint>
#include <QVector2D>


class QRect;
class Ball;


class Food : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int x READ x NOTIFY x_changed)
    Q_PROPERTY(int y READ y NOTIFY y_changed)
    Q_PROPERTY(int radius READ radius CONSTANT)
    Q_PROPERTY(bool enabled READ enabled NOTIFY enabled_changed)

public:
    // Default constructor, uses `game_size` to ensure creation in the game
    explicit Food(QRect *game_size, QObject *parent = nullptr);
    // Constructor used when players `fire` off, or create food from themselves
    explicit Food(Ball* ball_properties, QObject *parent = nullptr);

    // the standard mass used in creation
    static constexpr qreal initial_mass = 78.54;
    // create a "new" instance of food. Re-enables the food if it's been
    // disabled (consumed)
    void generate();

    int x();
    int y();
    qreal radius();
    qreal mass();
    bool enabled();
    QPoint position();
    void set_enabled(bool value);
    bool is_disabled();
    QVector2D intial_velocity();
    Ball *ball_properties();

    void move();

signals:
    void x_changed();
    void y_changed();
    void enabled_changed();

protected:
    void _connect_ball_property_signals();

private:
    Ball *_ball_properties;
    bool _enabled;
};

#endif // FOOD_H
