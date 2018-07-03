#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QTimer>

#include <QObject>
#include <QVariantList>

#include "gameinterface.h"
#include <QDebug>


int main(int argc, char *argv[])
{
    // FIXME: Now hardcoded into `GameInterface`
    // int game_height = 500;
    // int game_width = 500;

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    QQmlContext *root_context = engine.rootContext();

    // Step 1: get access to the root object
    // NOTE: the root object is currently the ApplicationWindow
    QObject *window_object = engine.rootObjects().first();

    // NOTE: probably need to get the cotent size instead
    qDebug() << window_object->property("height");
    QRect window_size(0, 0, window_object->property("height").toInt(), window_object->property("width").toInt());
    qDebug() << window_size;

    GameInterface game_interface(window_size);
    root_context->setContextProperty("viruses", game_interface.get_viruses());
    root_context->setContextProperty("feed", game_interface.get_food());
    root_context->setContextProperty("players", game_interface.get_players());
    root_context->setContextProperty("this_player", game_interface.get_this_player());

    QObject::connect(window_object, SIGNAL(height(int)), &game_interface, SLOT(debug(int)));


    QTimer interaction_timer;
    interaction_timer.setInterval(100);

    QObject::connect(&interaction_timer,
                     &QTimer::timeout,
                     &game_interface,
                     &GameInterface::check_game_object_interactions);

    interaction_timer.start();

    return app.exec();
}
