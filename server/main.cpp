#include <QCoreApplication>
#include <QWebSocketServer>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QWebSocketServer *server = new QWebSocketServer("eatme", QWebSocketServer::SecureMode);
    bool listening = server->listen(80);

    // http://doc.qt.io/qt-5/qtwebchannel-chatclient-qml-qmlchatclient-qml.html
    QObject::connect(server, &QWebSocketServer::newConnection, [](){
        QWebSocket *client = server->nextPendingConnection();
    });

    return a.exec();
}
