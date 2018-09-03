#include "ball.h"
#include <QRandomGenerator>
#include <QTimerEvent>
#include <QRect>
#include <QtMath>
#include <QDebug>

Ball::Ball(QRect *game_size, QVector2D initial_velocity, QPoint initial_position, qreal mass, QObject *parent)
    : QObject(parent)
    , _game_size(game_size)
    , _position(initial_position)
    , _mass(mass)
    , _velocity_ticks(-1)
    , _initial_velocity(initial_velocity)
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
    , _velocity_ticks(-1)
    , _initial_velocity(old_ball.initial_velocity())
{
}

QRect* Ball::game_size() const
{
    return _game_size;
}

void Ball::request_coordinates(QPoint mouse_position)
{
    if (abs(mouse_position.x()- x()) <= 1 && abs(mouse_position.y()-y()) <= 1)
        return;

    QPoint target_point = mouse_position - _position;
    QVector2D target = QVector2D(target_point).normalized();

    if (_velocity_ticks > 0) {
        target += _initial_velocity;
        _velocity_ticks -= 1;
    }
    else {
        target *= speed();
    }

    move(target);

}

void Ball::request_coordinates(QPoint mouse_position, Ball *touching_ball)
{
    QPoint target_point = mouse_position - _position;
    QVector2D to_target = QVector2D(target_point);
    QVector2D target_normalized = to_target.normalized();

    QPoint collision = _position - touching_ball->position();

    QVector2D collision_normal = QVector2D(collision).normalized();
    float into_collision_contribution;

    into_collision_contribution = qMin(QVector2D::dotProduct(target_normalized, collision_normal), (float) 0);
    collision_normal *= into_collision_contribution;

    QVector2D to_target_non_collide = target_normalized - collision_normal;

    if (_velocity_ticks > 0) {
        to_target_non_collide += _initial_velocity;
        _velocity_ticks -= 1;
    }
    else {
        to_target_non_collide *= speed();
    }

    // FIXME: add in some sort of clamp down to 3.
    // or could add in as an acceleration, to avoid the zig-zag
    move(to_target_non_collide);
}

void Ball::request_coordinates(QPoint mouse_position, QList<Ball *> touching_balls)
{
    QPoint target_calculation = mouse_position - _position;
    QVector2D to_target = QVector2D(target_calculation).normalized();
    QVector2D to_target_non_collide(to_target);

    for (Ball *touching_ball : touching_balls) {
        QPoint collision_calc = _position - touching_ball->position();
        QVector2D collision_normal = QVector2D(collision_calc).normalized();
        float into_collision_contribution;
        into_collision_contribution = qMin(QVector2D::dotProduct(to_target, collision_normal), (float) 0);
        collision_normal *= into_collision_contribution;
        to_target_non_collide -= collision_normal;
    }

    if (_velocity_ticks > 0) {
        to_target_non_collide += _initial_velocity;
        _velocity_ticks -= 1;
    }
    else {
        to_target_non_collide *= speed();
    }

    move(to_target_non_collide);
}

void Ball::add_mass(qreal mass)
{
    _mass += mass;
    emit radius_changed();
}

void Ball::remove_mass(qreal mass)
{
    _mass -= mass;
    emit radius_changed();
}

void Ball::set_mass(qreal mass)
{
    _mass = mass;
    emit radius_changed();
}

void Ball::move()
{
    if (_velocity_ticks < 0)
        return;

    _position.setX(_position.x() + _initial_velocity.x());
    _position.setY(_position.y() + _initial_velocity.y());

    _velocity_ticks--;
    validate_coordinates();
}

void Ball::move(QVector2D distance)
{
    QPoint move(distance.x(), distance.y());
    _position += move;
    validate_coordinates();
}

qreal Ball::speed()
{
    return qMax(_initial_player_speed * qPow(_mass / 2827.43, -0.439), 3.);

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

QPoint Ball::position() const
{
    return _position;
}

int Ball::x() const
{
    return _position.x();
}

bool Ball::is_touching(Ball *other)
{

    int radiuses = qPow(other->radius()+ radius(), 2);
    int diff_x = qPow(_position.x() - other->x(), 2);
    int diff_y = qPow(_position.y() - other->y(), 2);

    return radiuses > diff_x + diff_y;
}

int Ball::y() const
{
    return _position.y();
}

qreal Ball::mass() const
{
    return _mass;
}


qreal Ball::radius() const
{
    return qSqrt(_mass/M_PI);
}

QVector2D Ball::initial_velocity() const
{
    return _initial_velocity;
}

int Ball::velocity_ticks() const
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
