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

    void request_coordinates(int x, int y);
    void request_coordinates(int x, int y, Cell* touching_cells);
    void request_coordinates(int x, int y, QList<Cell*> touching_cells);

    bool is_object_touching(int object_x, int object_y, int object_radius);
    // NOTE: Could probably make a typedef of this
    QPointer<Cell> request_split(int mouse_x, int mouse_y);
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
