#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QTimer>

#include <QObject>
#include <QVariantList>

#include "virus.h"
#include "food.h"
#include "player.h"
#include "util.h"

#include <QDebug>

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
    // NOTE: this will need another layer probably.
    for(QVariant player_variant : players)
    {
        Player *player = qvariant_cast<Player*>(player_variant);
        for(QVariant food_variant : foods)
        {
            Food *food = food_variant.value<Food*>();
            if (!food->get_enabled())
                continue;
            if (touching(player, food))
            {
                player->add_mass(1);
                // food->generate();
                food->set_enabled(false);
            }
        }

        for (QVariant virus_variant: viruses)
        {
            Virus *virus = qvariant_cast<Virus*>(virus_variant);
            if (touching(player, virus))
            {
            }
        }

        for (QVariant other_player_variant : players)
        {
            Player *other_player = qvariant_cast<Player*>(other_player_variant);
            if (player == other_player)
                continue;
            else if (touching(player, other_player))
            {
            }
        }


    }
}


int main(int argc, char *argv[])
{
    // qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

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
