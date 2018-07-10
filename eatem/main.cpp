#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

// #include <QObject>
#include "gameinterface.h"

#include <QDebug>


int main(int argc, char *argv[])
{
    // FIXME: Now hardcoded into `GameInterface`
    // int game_height = 500;
    // int game_width = 500;

    // Enable High Dpi Scaling because ....
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    // Create our application, which controls our event loop
    QGuiApplication app(argc, argv);

    qmlRegisterType<GameInterface>("GameInterfaces", 1, 0, "GameInterface");

    // Create our QML application engine, which handles our QML
    QQmlApplicationEngine engine;
    // Load our `main.qml` page
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    // Check to see if we loaded the file correctly
    if (engine.rootObjects().isEmpty())
        // if we didn't load correctly, exit the main loop with the error/integer, `-1`
        return -1;

    // Get the root context from our QML engine
    // QQmlContext *root_context = engine.rootContext();
    // ------------------------------------------

    // Create our `GameInterface`, an interface which abstracts some of the game creation
    // aspects for us,  including:
    // 1. creating food
    // 2. creating viruses
    // 3. Creating a list of players

    // NOTE: hack to get around delayed creation
    // QObject::connect(&game_interface, &GameInterface::create_game_objects, [&root_context, &game_interface](){
        // We'll get out everything from the `game_interface` creates for us, and track
        // it in the QML context using the `setContextProperty` method.
        // The first argument in the method call is the name/accessor we'll use in QML
    // root_context->setContextProperty("viruses", game_interface.get_viruses());
    // root_context->setContextProperty("feed", game_interface.get_food());
    // });

    // root_context->setContextProperty("players", game_interface.get_players());

    // Step 1: get access to the root object
    // NOTE: the root object is currently the ApplicationWindow
    // QObject *application_object = engine.rootObjects().first();
    // QQuickItem *content_item = QQmlProperty::read(application_object, "contentItem").value<QQuickItem *>();

    /*
    QObject::connect(content_item, &QQuickItem::heightChanged, [&game_interface, content_item](){
        game_interface.set_game_height(content_item->height());
    });

    QObject::connect(content_item, &QQuickItem::widthChanged, [&game_interface, content_item](){
        game_interface.set_game_width(content_item->width());
    });
    */

    // We need to increment the game in a set amount of time.
    // We use a `QTimer` called `game_timestep` to trigger the game incremention
    /*
    QTimer game_timestep;
    // the interval for our time step is 100 milliseconds
    game_timestep.setInterval(100);

    // We connect the game's timestep `timeout` function to our `game_interface`,
    // specifically the `check_game_object_interactions` function
    QObject::connect(&game_timestep,
                     &QTimer::timeout,
                     game_interface,
                     &GameInterface::increment_game_step);

    // Start our game timestep
    game_timestep.start();
    */
    // execute the event loop. Return the result of the event loop if it stops.
    return app.exec();
}
