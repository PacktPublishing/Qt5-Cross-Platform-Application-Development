import QtQuick 2.11
import QtQuick.Controls 1.4
import GameInterfaces 1.0

import "app.js" as App


ApplicationWindow {
    id: window
    title: "eatem"
    visible: true

    GameInterface {
        id: game_interface
        Component.onCompleted: {
            game_interface.set_game_size(1000, 1000);
            canvas.this_player = game_interface.get_this_player("AUTHENTICATION");
        }

    }

    Canvas {
        id: canvas
        anchors.fill: parent
        focus: true
        contextType: "2d"
        property color clear_color: 'white'
        property var context
        property var feed: game_interface.food
        property var players: game_interface.players
        property var viruses: game_interface.viruses
        property var this_player
        property int gridSize: 30
        property string authentication

        onPaint: {
            context = getContext("2d");
            game_loop();
        }

        function game_loop()
        {
            // You can't block with JavaScript...
            // So normally this would look like a `while (true):` loop
            // But since you never go back into the event loop with that pattern,
            // your user interface would hang and become unresponsive.
            // So instead we recursively register this same function
            // on the next animation frame, using the `requestAnimationFrame` method
            requestAnimationFrame(game_loop);

            // Set the fill style to clear the background
            context.fillStyle = clear_color;

            // Clear the background
            context.clearRect(0, 0, canvas.width, canvas.height);
            // draw_grid();

            // draw the food
            App.draw_food(context, feed, this_player);

            // and draw the players
            App.draw_players(context, players, this_player);

            App.draw_viruses(context, viruses, this_player);
        }

        Keys.onSpacePressed: {
            var x_y = translate_mouse(mouse);
            this_player.request_split(x_y[0], x_y[1], "AUTHENTICATION");
        }

        Keys.onPressed: {
            if (event.key === Qt.Key_W)
            {
                var x_y = translate_mouse(mouse);
                this_player.request_fire_food(x_y[0], x_y[1], "AUTHENTICATION");
                event.accepted = true
            }
        }

        MouseArea {
            id: mouse
            anchors.fill: parent
            hoverEnabled: true
        }

        function translate_mouse(mouse)
        {
            return [mouse.mouseX - width/2 + this_player.x,
                    mouse.mouseY - height/2 + this_player.y];
        }

        Timer {
            id: lineTimer
            interval: 10
            repeat: true
            running: true
            onTriggered: {
                var x_y = canvas.translate_mouse(mouse);
                canvas.this_player.request_coordinates(x_y[0], x_y[1], "AUTHENTICATION");
            }
        }
    }
}
