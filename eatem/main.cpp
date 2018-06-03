#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QObject>
#include <QVariantList>
#include "virus.h"
#include "food.h"
#include "player.h"
#include "util.h"

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

QVariantList create_food()
{
    QVariantList result;
    int number = 100;
    for(int i=0; i<number; i++)
        result.append(QVariant::fromValue<Food*>(new Food()));
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
            if (touching(player, food))
            {
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

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    QVariantList food = create_food();
    QVariantList viruses = create_viruses();
    QVariantList players;

    Player *this_player = new Player();
    players.append(QVariant::fromValue<Player*>(this_player));

    QQmlContext *root_context = engine.rootContext();

    root_context->setContextProperty("viruses", viruses);
    root_context->setContextProperty("food", food);
    root_context->setContextProperty("players", players);
    root_context->setContextProperty("this_player", this_player);

    return app.exec();
}
