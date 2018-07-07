#include "gameinterface.h"


GameInterface::GameInterface(QObject *parent)
    : QObject(parent)
    , _websocket_server(new QWebSocketServer("Eatem", QWebSocketServer::NonSecureMode))
    , _webchannel(new QWebChannel())
    , _client_wrapper(new WebSocketClientWrapper(_websocket_server))
{
    // TODO: make configurable
    bool listening = _websocket_server->listen(QHostAddress::LocalHost, 5555);

    Q_UNUSED(listening)

    _game_size.setHeight(500);
    _game_size.setWidth(500);

    create_food();
    create_viruses();

    _webchannel->registerObject("feed", _food);
    _webchannel->registerObject("viruses", _viruses);
    _webchannel->registerObject("players", _players);

    connect(_websocket_server, &QWebSocketServer::newConnection,
            this, &GameInterface::handle_new_connection);
    connect(this, &GameInterface::client_connected, _webchannel, &QWebChannel::connectTo);
}

void GameInterface::handle_new_connection()
{
    WebSocketTransport *trasnport = new WebSocketTransport(_websocket_server->nextPendingConnection());
    emit client_connected(transport);
}

void GameInterface::create_viruses()
{
    int number = 5;
    for(int i=0; i<number; i++)
        _viruses.append(QVariant::fromValue<Virus*>(new Virus()));
}


void GameInterface::create_food(int number)
{
    // FIXME: hardcoding magic numbers
    for(int i=0; i<number; i++)
        _food.append(QVariant::fromValue<Food*>(new Food(_game_size)));
}

QVariantList GameInterface::get_food()
{
    return _food;
}

QVariantList GameInterface::get_viruses()
{
    return _viruses;
}

Player* GameInterface::get_this_player()
{
    return _this_player;
}

QVariantList GameInterface::get_players()
{
    return _players;
}

void GameInterface::increment_game_step()
{
    check_game_object_interactions();
}

// howto handle collisions
// https://www.reddit.com/r/gamedev/comments/6aqu5x/how_do_games_like_agario_handle_collisions/
void GameInterface::check_game_object_interactions()
{
    // For each Player QVariant in our QVariantList `_players`...
    for(QVariant player_variant : _players)
    {
        // cast each player variant into into a `Player` pointer
        Player *player = player_variant.value<Player*>();

        // Now iterate through every food variant
        for(QVariant food_variant : _food)
        {
            // cast the food variant into into a `Food` pointer
            Food *food = food_variant.value<Food*>();
            if (food->is_disabled())
                continue;
            player->handle_touch(food);
        }

        // Now iterate through every virus variant
        for (QVariant virus_variant: _viruses)
        {
            // cast the virius variant into into a `Virus` pointer
            Virus *virus = virus_variant.value<Virus*>();
            player->handle_touch(virus);
        }

        // Now iterate through every other player variant
        for (QVariant other_player_variant : _players)
        {
            // cast the other player variant into into a `Virus` pointer
            Player *other_player = other_player_variant.value<Player*>();
            if (player == other_player)
                continue;
            player->handle_touch(other_player);
        }
    }
}

void GameInterface::set_game_height(int height)
{
    qDebug() << height;
    _game_size.setHeight(height);
}

void GameInterface::set_game_widget(int width)
{
    _game_size.setWidth(width);
}
