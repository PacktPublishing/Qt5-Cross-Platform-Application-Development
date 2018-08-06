#ifndef CELL_H
#define CELL_H

#include <QObject>
#include <QPointer>
#include <QVector2D>

class Food;
class Player;
class Ball;

class Cell : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int x READ x NOTIFY x_changed)
    Q_PROPERTY(int y READ y NOTIFY y_changed)
    Q_PROPERTY(int radius READ radius NOTIFY radius_changed)

public:
    explicit Cell(QRect *game_size, QObject *parent);
    explicit Cell(QVector2D start_position, QVector2D velocity, qreal mass, QRect *game_size, QObject *parent);
    static constexpr qreal initial_mass = 2827.43;

    void add_mass(qreal amount);

    void request_coordinates(QVector2D mouse_position);
    void request_coordinates(QVector2D target_position, Cell* touching_cells);
    void request_coordinates(QVector2D position, QList<Cell*> touching_cells);

    bool is_touching(Ball *other);

    // NOTE: Could probably make a typedef of this
    QPointer<Cell> request_split(QVector2D mouse_position);
    QPointer<Food> request_fire_food(QVector2D mouse_position);
    void set_mass(qreal mass);

    // Getters
    int x();
    int y();
    qreal radius();
    qreal mass();
    QVector2D position();
    qreal velocity();
    Ball* ball_properties();


protected:
    void _connect_ball_property_signals();

signals:
    void x_changed();
    void y_changed();
    void radius_changed();

private:
    Ball *_ball_properties;
};

#endif // CELL_H
