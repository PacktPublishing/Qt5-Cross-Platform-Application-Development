#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QtMath>
#include <QVariant>
#include <QColor>
#include <QDebug>
#include "cell.h"


class Player : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QColor hue READ get_hue)

public:
    explicit Player(QObject *parent = nullptr);
    QColor get_hue();

    Q_INVOKABLE void request_coordinates(int x, int y);



private:
    void validate_coordinates();
    QColor _hue;
    QVariantList _cells;
};

#endif // PLAYER_H
