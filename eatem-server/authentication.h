#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H
#include <QObject>
#include <QString>

class Authentication: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString auth_code READ auth_code NOTIFY update_auth)

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

signals:
    void update_auth();

private:
    QString _auth;

};
#endif // AUTHENTICATION_H
