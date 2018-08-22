#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QWebChannel>
#include <QWebSocketServer>
#include <QRandomGenerator>
#include "websockettransport.h"

#include "gameinterface.h"


class Authentication;


class Game : public QObject
{
    Q_OBJECT
public:
    explicit Game(QObject *parent = nullptr);
    bool start();

protected slots:
    void handle_new_connection();

signals:
    void client_connected(WebSocketTransport *client);

private:
    GameInterface *_game_interface;
    // The Web Channel
    QWebChannel *_webchannel;
     // Web Socket Server
    QWebSocketServer *_websocket_server;
    Authentication *_auth;
};

#endif // GAME_H
