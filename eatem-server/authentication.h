#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H
#include <QObject>
#include <QString>

class Authentication: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString auth_code READ auth_code NOTIFY change)

public:
    explicit Authentication(QObject *parent = nullptr)
        : QObject(parent)
    {
    }

    QString auth_code()
    {
        return QString();
    }


signals:
    void change(QString new_auth);

};
#endif // AUTHENTICATION_H
