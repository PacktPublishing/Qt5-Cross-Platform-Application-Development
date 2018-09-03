#ifndef BALL_H
#define BALL_H

#include <QObject>
#include <QVector2D>


class QTimerEvent;


class Ball : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int x READ x NOTIFY x_changed)
    Q_PROPERTY(int y READ y NOTIFY y_changed)
    Q_PROPERTY(int radius READ radius NOTIFY radius_changed)

public:
    Ball(QRect *game_size,
         QVector2D initial_velocity,
         QPoint initial_position,
         qreal mass,
         QObject *parent = nullptr);

    explicit Ball(QRect *game_size, qreal mass, QObject *parent = nullptr);
    explicit Ball(const Ball&);

    int x() const;
    int y() const;
    qreal radius() const;
    qreal mass() const;
    QPoint position() const;
    QRect* game_size() const;
    QVector2D initial_velocity() const;
    int velocity_ticks() const;
    void set_initial_velocity(QVector2D velocity);
    void set_coordinates_random();

    void request_coordinates(QPoint mouse_position);
    void request_coordinates(QPoint mouse_position, Ball* touching_ball);
    void request_coordinates(QPoint mouse_position, QList<Ball *> touching_balls);

    void add_mass(qreal mass);
    void remove_mass(qreal mass);
    void set_mass(qreal mass);
    void move();

    void set_velocity_ticks(int ticks);
    bool is_touching(Ball *other);

protected:
    void validate_coordinates();
    void move(QVector2D distance);

    static constexpr qreal _initial_player_speed = 6.;
    qreal speed();


signals:
    void x_changed();
    void y_changed();
    void radius_changed();

private:
    QRect *_game_size;
    QPoint _position;
    qreal _mass;

    int _velocity_ticks;
    QVector2D _initial_velocity;
};

#endif // BALL_H
