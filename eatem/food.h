#ifndef FOOD_H
#define FOOD_H

#include <QObject>
#include <QRandomGenerator>
#include <QVector2D>
#include <QRect>
#include <QColor>


class Food : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int x READ x)
    Q_PROPERTY(int y READ y)
    Q_PROPERTY(int radius READ radius)
    Q_PROPERTY(bool enabled READ enabled)
    Q_PROPERTY(QColor hue READ hue)
public:
    explicit Food(QRect game_size, QObject *parent = nullptr);

    void generate();

    int x() {return _position.x();}
    int y() {return _position.y();}
    int radius() {return _radius;}
    QColor hue() { return _hue;}
    bool enabled() {return _enabled;}
    void set_enabled(bool value) {_enabled = value;}
    // NOTE: Figure out if this is needed
    bool is_disabled() {return !_enabled;}


private:
    QVector2D _position;
    int _radius;
    QColor _hue;
    bool _enabled;
    QRect _game_size;
};

#endif // FOOD_H
