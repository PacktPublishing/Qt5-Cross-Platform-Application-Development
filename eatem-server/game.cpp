#include "game.h"

namespace {
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
}

Game::Game(QObject *parent)
    : QObject(parent)
    , _websocket_server(new QWebSocketServer("Eatem", QWebSocketServer::NonSecureMode))
    , _webchannel(new QWebChannel())
    , _game_interface(new GameInterface(this))
{
    bool listening = _websocket_server->listen(QHostAddress::LocalHost, 5555);
    Q_UNUSED(listening)
    _webchannel->registerObject("interface", _game_interface);
    // NOTE: intentional memory link
    _webchannel->registerObject("authentication", new QString());
    connect(_websocket_server, &QWebSocketServer::newConnection,
            this, &GameInterface::handle_new_connection);

    connect(this, &GameInterface::client_connected, _webchannel, &QWebChannel::connectTo);

}

void Game::handle_new_connection()
{
    QWebSocket *web_socket = _websocket_server->nextPendingConnection();
    // QString origin = web_socket->origin();
    WebSocketTransport *transport = new WebSocketTransport(web_socket);
    emit client_connected(transport);
    Player *new_player = new Player(GetRandomString());
    // FIXME: Figure out a better way to manage this memory
    // currently connecting the WebSocket destroyed to the Player delete Later slot
    connect(web_socket, &QWebSocket::destroyed, new_player, &Player::deleteLater);
    QJsonObject message;
    /*
    Set property type to be property update
    "type": 2
    set the object to be authentication
    "object": "authentication"
    "property": property Index
    "value" my_authentication_string
    */
    transport->sendMessage(message);
    _players.append(QVariant::fromValue<Player*>(new_player));
}
