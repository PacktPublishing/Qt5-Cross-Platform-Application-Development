#ifndef CELL_H
#define CELL_H

#include <QObject>
#include <QPointer>
#include <QVector2D>

class Player;

class Cell : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int x READ x)
    Q_PROPERTY(int y READ y)
    Q_PROPERTY(int radius READ radius)
    Q_PROPERTY(qreal mass READ mass)

public:
    explicit Cell(QRect *game_size, QObject *parent);
    explicit Cell(QVector2D start_position, QVector2D velocity, qreal mass, QRect *game_size, QObject *parent);
    static constexpr qreal initial_mass = 2827.43;

    void add_mass(qreal amount);

    void request_coordinates(QVector2D mouse_position);
    void request_coordinates(QVector2D target_position, Cell* touching_cells);
    void request_coordinates(QVector2D position, QList<Cell*> touching_cells);

    bool is_object_touching(QPoint center_point, int object_radius);
    bool is_object_touching(QVector2D other_center, int object_radius);
    // NOTE: Could probably make a typedef of this
    QPointer<Cell> request_split(QVector2D mouse_position);
    // qreal calc_radians(int x, int y);

    // Getters
    int x();
    int y();
    qreal radius();
    qreal mass();
    QVector2D position();

private:
    void validate_coordinates();

    QVector2D _position;
    QRect *_game_size;
    qreal _mass;

    // NOTE: used when firing
    QVector2D _velocity;

    int _velocity_ticks;
};

#endif // CELL_H
