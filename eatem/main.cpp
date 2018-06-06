#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QTimer>

#include <QObject>
#include <QVariantList>

#include "virus.h"
#include "food.h"
#include "player.h"

#include <QDebug>

// Need to declare these pointers as metatypes
// so we can add them into `QVariants`.
// Need to add them into `QVaraints` so that we can pass them to QML.
Q_DECLARE_METATYPE(Virus *)
Q_DECLARE_METATYPE(Food *)
Q_DECLARE_METATYPE(Player *)


QVariantList create_viruses()
{
    QVariantList result;
    int number = 5;
    for(int i=0; i<number; i++)
        result.append(QVariant::fromValue<Virus*>(new Virus()));
    return result;
}


QVariantList create_food(int game_width, int game_height, int number=500)
{
    QVariantList result;
    for(int i=0; i<number; i++)
        result.append(QVariant::fromValue<Food*>(new Food(game_width, game_height)));
    return result;

}


void check_interactions(QVariantList players, QVariantList viruses, QVariantList foods)
{
    // For each Player variant ...
    for(QVariant player_variant : players)
    {
        // cast each player variant into into a `Player` pointer
        Player *player = player_variant.value<Player*>();

        // Now iterate through every food variant
        for(QVariant food_variant : foods)
        {
            // cast the food variant into into a `Food` pointer
            Food *food = food_variant.value<Food*>();
            if (food->is_disabled())
                continue;
            player->handle_touch(food);
        }

        // Now iterate through every virus variant
        for (QVariant virus_variant: viruses)
        {
            // cast the virius variant into into a `Virus` pointer
            Virus *virus = virus_variant.value<Virus*>();
            player->handle_touch(virus);
        }

        // Now iterate through every other player variant
        for (QVariant other_player_variant : players)
        {
            // cast the other player variant into into a `Virus` pointer
            Player *other_player = other_player_variant.value<Player*>();
            if (player == other_player)
                continue;
            player->handle_touch(other_player);
        }
    }
}


int main(int argc, char *argv[])
{
    int game_height = 500;
    int game_width = 500;

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    QVariantList food = create_food(game_width, game_height, 100);
    QVariantList viruses = create_viruses();
    QVariantList players;

    Player *this_player = new Player();
    players.append(QVariant::fromValue<Player*>(this_player));

    QQmlContext *root_context = engine.rootContext();

    root_context->setContextProperty("viruses", viruses);
    root_context->setContextProperty("feed", food);
    root_context->setContextProperty("players", players);
    root_context->setContextProperty("this_player", this_player);

    QTimer my_timer;
    my_timer.setInterval(100);
    QObject::connect(&my_timer, &QTimer::timeout, [players, viruses, food](){
        check_interactions(players, viruses, food);
    });

    my_timer.start();
    return app.exec();
}
