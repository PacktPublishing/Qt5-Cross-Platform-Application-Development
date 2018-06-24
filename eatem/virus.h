#ifndef VIRUS_H
#define VIRUS_H

#include <QObject>
#include <QRandomGenerator>

class Virus : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int x READ x)
    Q_PROPERTY(int y READ y)
    Q_PROPERTY(int radius READ radius)

public:
    explicit Virus(QObject *parent = nullptr);

    int x();
    int y();
    int radius();


private:
    int _x;
    int _y;
    int _radius;
};

#endif // VIRUS_H
