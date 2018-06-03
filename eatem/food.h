#ifndef FOOD_H
#define FOOD_H

#include <QObject>

class Food : public QObject
{
    Q_OBJECT
public:
    explicit Food(QObject *parent = nullptr);

signals:

public slots:
};

#endif // FOOD_H