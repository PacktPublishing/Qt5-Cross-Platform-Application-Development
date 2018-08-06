#ifndef BALL_H
#define BALL_H

#include <QObject>
#include <QVector2D>


class QTimerEvent;


class Ball : public QObject
{
    Q_OBJECT

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

    void request_coordinates(QVector2D mouse_position);

    void add_mass(qreal mass);
    void set_mass(qreal mass);

    void set_velocity_ticks(int ticks);
    // FIXME: Better name?
    void start_counting_velocity_ticks();
    bool is_touching(Ball *other);

protected:
    void timerEvent(QTimerEvent *event);
    void validate_coordinates();

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
