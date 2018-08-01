#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H
#include <QObject>
#include <QString>

class Authentication: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString auth_code READ auth_code NOTIFY change)

public:
    explicit Authentication(QString authentication = QString(), QObject *parent = nullptr)
        : QObject(parent)
        , _auth(authentication)
    {
    }

    QString auth_code()
    {
        return _auth;
    }

    void set_string(QString str)
    {
        emit change(str);
    }

signals:
    void change(QString new_auth);

private:
    QString _auth;

};
#endif // AUTHENTICATION_H
