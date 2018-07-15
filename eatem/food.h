#ifndef FOOD_H
#define FOOD_H

#include <QObject>
#include <QPoint>
#include <QColor>
#include <QVector2D>

class QRect;

class Food : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int x READ x)
    Q_PROPERTY(int y READ y)
    Q_PROPERTY(int radius READ radius)
    Q_PROPERTY(bool enabled READ enabled)
    Q_PROPERTY(QColor hue READ hue)

public:
    // Default constructor, uses `game_size` to ensure creation in the game
    explicit Food(QRect *game_size, QObject *parent = nullptr);
    // Constructor used when players `fire` off, or create food from themselves
    explicit Food(QVector2D initial_velocity,
                  QPoint initial_position,
                  qreal mass,
                  QRect *game_size,
                  QObject *parent = nullptr);

    // the standard mass used in creation
    static constexpr qreal initial_mass = 78.54;
    // create a "new" instance of food. Re-enables the food if it's been
    // disabled (consumed)
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

protected:
    // when food is `fired` by players, it has velocity and thus needs an event handeling
    void timerEvent(QTimerEvent *event);

private:
    static constexpr qreal _radius = 5.0;

    QPoint _position;
    QColor _hue;
    bool _enabled;
    QRect *_game_size;
    qreal _mass;
    int _velocity_ticker;
    QVector2D _initial_velocity;
    int _timer_id;
};

#endif // FOOD_H
