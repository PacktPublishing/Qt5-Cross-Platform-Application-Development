#include <QCoreApplication>
#include "game.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Game game;
    bool running = game.start();

    if (!running)
    {
        qCritical("Server socket already in use");
        return -1;
    }

    return a.exec();
}
