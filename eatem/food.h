#ifndef FOOD_H
#define FOOD_H

#include <QObject>
#include <QRandomGenerator>
#include <QColor>


class Food : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int x READ get_x)
    Q_PROPERTY(int y READ get_y)
    Q_PROPERTY(int radius READ get_radius)
    Q_PROPERTY(bool enabled READ get_enabled)
    Q_PROPERTY(QColor hue READ get_hue)
public:
    explicit Food(int game_width, int game_height, QObject *parent = nullptr);

    void generate();

    int get_x() {return _x;}
    int get_y() {return _y;}
    int get_radius() {return _radius;}
    QColor get_hue() { return _hue;}
    bool get_enabled() {return _enabled;}
    bool set_enabled(bool value) {_enabled = value;}
    bool is_disabled() {return !_enabled;}


private:
    int _x;
    int _y;
    int _radius;
    QColor _hue;
    bool _enabled;

    int _game_height;
    int _game_width;
};

#endif // FOOD_H
