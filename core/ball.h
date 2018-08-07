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
         QVector2D intial_velocity,
         QPoint initial_position,
         qreal mass,
         QObject *parent = nullptr);

    explicit Ball(QRect *game_size, qreal mass, QObject *parent = nullptr);
    explicit Ball(const Ball&);

    int x();
    int y();
    qreal radius();
    qreal mass();
    QPoint position();
    QRect* game_size();
    QVector2D intial_velocity();
    int velocity_ticks();
    void set_initial_velocity(QVector2D velocity);
    void set_coordinates_random();

    void request_coordinates(QPoint mouse_position);
    void request_coordinates(QPoint mouse_position, Ball* touching_ball);
    void request_coordinates(QPoint mouse_position, QList<Ball *> touching_balls);

    void add_mass(qreal mass);
    void set_mass(qreal mass);

    void set_velocity_ticks(int ticks);
    // FIXME: Better name?
    void start_counting_velocity_ticks();
    bool is_touching(Ball *other);

protected:
    void timerEvent(QTimerEvent *event);
    void validate_coordinates();

    // NOTE: The game interval timer blocks the event loop long enough
    // that this number works and prevents stuttering associated with not
    // being associated with the game interval timer. Not a great hack
    // if the game was better optimized. All objects moving of their own
    // speed would go a LOT faster.
    static int _timer_interval = 10;


signals:
    void x_changed();
    void y_changed();
    void radius_changed();

private:
    QRect *_game_size;
    QPoint _position;
    qreal _mass;

    int _timer_id;
    int _velocity_ticks;
    QVector2D _initial_velocity;
};

#endif // BALL_H
