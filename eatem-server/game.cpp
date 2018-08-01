#include "game.h"
#include "player.h"
#include <QJsonObject>
#include <QWebSocket>
#include <QJsonArray>
#include <QTimer>
#include "authentication.h"

// https://stackoverflow.com/questions/18862963/qt-c-random-string-generation
QString GetRandomString()
{
   const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
   const int randomStringLength = 12; // assuming you want random strings of 12 characters
   const int length_characters = possibleCharacters.length();
   QRandomGenerator random = QRandomGenerator::securelySeeded();
   QString randomString;
   for(int i=0; i<randomStringLength; ++i)
   {
       int index = random.bounded(length_characters);
       QChar nextChar = possibleCharacters.at(index);
       randomString.append(nextChar);
   }
    return randomString;
}

Game::Game(QObject *parent)
    : QObject(parent)
    , _webchannel(new QWebChannel())
    , _websocket_server(new QWebSocketServer("Eatem", QWebSocketServer::NonSecureMode))
    , _game_interface(new GameInterface(this))
    , _auth(new Authentication())
{
    _webchannel->registerObject("interface", _game_interface);
    _webchannel->registerObject("authentication", _auth);
    connect(_websocket_server, &QWebSocketServer::newConnection,
            this, &Game::handle_new_connection);

    connect(this, &Game::client_connected, _webchannel, &QWebChannel::connectTo);

}

void Game::start()
{
    bool listening = _websocket_server->listen(QHostAddress::LocalHost, 5555);
    qDebug() << listening;
    if (!listening)
        return;

    // NOTE: should think about setting game interval in here too?
    _game_interface->set_game_size(1000, 1000);
    _game_interface->start_game();
}

QJsonObject GetMessage(QString authentication)
{
    QJsonObject message;
    // FIXME: Should probably use consts instead of hardcoding strings in here

    message["auth"] = authentication;
    message["type"] = 11;
    return message;
}

void Game::handle_new_connection()
{
    QWebSocket *web_socket = _websocket_server->nextPendingConnection();
    WebSocketTransport *transport = new WebSocketTransport(web_socket);

    emit client_connected(transport);
    QString authentication = GetRandomString();
    Player *new_player = new Player(authentication, _game_interface->game_size(), _game_interface);
    _game_interface->add_player(new_player);
    qDebug() << _auth->auth_code();

    // https://code.woboq.org/qt5/qtwebchannel/src/webchannel/qmetaobjectpublisher.cpp.html#290
    // NOTE: might want to take the "specific update" route as seen in the code
    QJsonObject message = GetMessage(authentication);
    // {"data":[{"object":"authentication","properties":{"1":"QUaP1SVsQhRL"},"signals":{"5":[]}}],"type":2}
    // FIXME: this signal triggers the `connectTo` function from QWebChannel. It clobbers the authenticaiton
    // Note that there is no property cache at this time
    transport->sendMessage(message);

    // currently connecting the WebSocket destroyed to the Player delete Later slot
    connect(web_socket, &QWebSocket::destroyed, [this, new_player](){
        _game_interface->remove_player(new_player);
        new_player->deleteLater();});
}
