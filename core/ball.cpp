#include "ball.h"
#include <QRandomGenerator>
#include <QTimerEvent>


Ball::Ball(QRect *game_size, QVector2D initial_velocity, QPoint initial_position, qreal mass, QObject *parent)
    : QObject(parent)
    , _game_size(game_size)
    , _mass(mass)
    , _initial_velocity(initial_velocity)
    , _velocity_ticks(-1)
{
}

Ball::Ball(QRect *game_size, qreal mass, QObject *parent)
    : QObject(parent)
    , _game_size(game_size)
    , _mass(mass)
    , _velocity_ticks(-1)
{
}

Ball::Ball(QRect *game_size, qreal mass, QObject *parent)
    : QObject(parent)
    , _game_size(game_size)
    , _mass(mass)
    , _velocity_ticks(-1)
{
}

Ball::Ball(const Ball &old_ball)
    : QObject(old_ball.parent())
    , _game_size(old_ball.game_size())
    , _position(old_ball.position())
    , _mass(old_ball.mass())
    , _initial_velocity(old_ball.intial_velocity())
    , _velocity_ticks(-1)
{
}

QRect* Ball::game_size()
{
    return _game_size;
}

void Ball::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == _timer_id)
    {
        _position.setX(_position.x() + _initial_velocity.x());
        _position.setY(_position.y() + _initial_velocity.y());
        if (_velocity_ticks == 0)
            killTimer(_timer_id);

        _velocity_ticks--;
        validate_coordinates();
    }
}

void Ball::request_coordinates(QPoint mouse_position)
{
    if (abs(mouse_position.x()- x()) <= 1 && abs(mouse_position.y()-y()) <= 1)
        return;

    // FIXME
    QVector2D target = mouse_position - _position;
    target.normalize();

    if (_velocity_ticks > 0) {
        target += _velocity;
        _velocity_ticks -= 1;
    }
    else {
        target *= velocity();
    }

    _position += target;

}

void Ball::validate_coordinates()
{
    // Check to see if we're within the game width
    if (_position.x() < 0)
        _position.setX(0);
    // Check to see if we're within the game width
    else if (_position.x() > _game_size->width())
        _position.setX(_game_size->width());

    // Check to see if we're within the game height
    if (_position.y() < 0)
        _position.setY(0);
    // Check to see if we're within the game height
    else if (_position.y() > _game_size->height())
        _position.setY(_game_size->height());

    emit x_changed();
    emit y_changed();
}

QPoint Ball::position()
{
    return _position;
}

int Ball::x()
{
    return _position.x();
}

bool Ball::is_touching(Ball *other)
{

    int radiuses = qPow(other->radius()+ radius(), 2);
    int diff_x = qPow(_position.x() - other.x(), 2);
    int diff_y = qPow(_position.y() - other.y(), 2);

    return radiuses > diff_x + diff_y;
}

int Ball::y()
{
    return _position.y();
}

qreal Ball::mass()
{
    return _mass;
}


qreal Ball::radius()
{
    return qSqrt(_mass/M_PI);
}

QVector2D Ball::intial_velocity()
{
    return _initial_velocity;
}

int Ball::velocity_ticks()
{
    return _velocity_ticks;
}

void Ball::set_initial_velocity(QVector2D velocity)
{
    _initial_velocity = velocity;
}

void Ball::set_coordinates_random()
{
    QRandomGenerator random = QRandomGenerator::securelySeeded();
    _position.setX(random.bounded(_game_size->width()));
    _position.setY(random.bounded(_game_size->height()));
}

void Ball::set_velocity_ticks(int ticks)
{
    _velocity_ticks = ticks;
}

void Ball::start_counting_velocity_ticks()
{
    _timer_id = startTimer(10);
}
