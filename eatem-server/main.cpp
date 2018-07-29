#include <QCoreApplication>
#include "game.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Game game;

    return a.exec();
}
