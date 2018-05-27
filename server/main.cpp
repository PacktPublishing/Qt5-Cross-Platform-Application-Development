#include <QCoreApplication>
#include <QWebSocketServer>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QWebSocketServer *server = new QWebSocketServer("eatme", QWebSocketServer::SecureMode);
    bool listening = server->listen(80);

    QObject::connect(server, &QWebSocketServer::newConnection, [](){
        QWebSocket *client = server->nextPendingConnection();
    });

    return a.exec();
}
