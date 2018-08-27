#ifndef CELL_H
#define CELL_H

#include <QObject>
#include <QPointer>


class Food;
class Ball;


class Cell : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int x READ x NOTIFY x_changed)
    Q_PROPERTY(int y READ y NOTIFY y_changed)
    Q_PROPERTY(int radius READ radius NOTIFY radius_changed)

public:
    explicit Cell(QRect *game_size, QObject *parent);
    explicit Cell(Ball *ball_properties, QObject *parent);
    static constexpr qreal initial_mass = 2827.43;

    void add_mass(qreal amount);

    void request_coordinates(QPoint mouse_position);
    void request_coordinates(QPoint target_position, Ball* touching_ball);
    void request_coordinates(QPoint position, QList<Cell *> touching_cells);

    bool is_touching(Ball *other);

    // NOTE: Could probably make a typedef of this
    QPointer<Cell> request_split(QPoint mouse_position);
    QPointer<Food> request_fire_food(QPoint mouse_position);
    void set_mass(qreal mass);

    void move();

    // Getters
    int x();
    int y();
    void eat_food(Food* food);
    qreal radius();
    qreal mass();
    QPoint position();
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
