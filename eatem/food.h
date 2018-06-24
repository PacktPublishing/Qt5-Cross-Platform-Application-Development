#ifndef FOOD_H
#define FOOD_H

#include <QObject>
#include <QRandomGenerator>
#include <QColor>


class Food : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int x READ x)
    Q_PROPERTY(int y READ y)
    Q_PROPERTY(int radius READ get_radius)
    Q_PROPERTY(bool enabled READ get_enabled)
    Q_PROPERTY(QColor hue READ get_hue)
public:
    explicit Food(int game_width, int game_height, QObject *parent = nullptr);

    void generate();

    int x() {return _x;}
    int y() {return _y;}
    int radius() {return _radius;}
    QColor hue() { return _hue;}
    bool enabled() {return _enabled;}
    bool set_enabled(bool value) {_enabled = value;}
    // NOTE: Figure out if this is needed
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
