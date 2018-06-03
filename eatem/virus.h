#ifndef VIRUS_H
#define VIRUS_H

#include <QObject>

class Virus : public QObject
{
    Q_OBJECT
public:
    explicit Virus(QObject *parent = nullptr);

signals:

public slots:
};

#endif // VIRUS_H