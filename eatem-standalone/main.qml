import QtQuick 2.11
import QtQuick.Controls 1.4

import "../core/app.js" as App


ApplicationWindow {
    id: window
    title: "eatem"
    visible: true

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
        property var this_player: game_interface.get_player("AUTH")
        property int gridSize: 30
        property string authentication

        Component.onCompleted: {
            for (var i = 0; i < canvas.feed.length; i++){
                var hue_num = Math.round(Math.random() * 360)
                canvas.feed[i].hue = 'hsl(' + hue_num + ', 100%, 50%)';
            }

            canvas.players = game_interface.players;
            for (i = 0; i < canvas.players.length; i++){
                var hue_num = Math.round(Math.random() * 360)
                canvas.players[i].hue = 'hsl(' + hue_num + ', 100%, 50%)';
            }

        }

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
            App.draw_objects(context,
                                         feed,
                                         players,
                                         viruses,
                                         this_player,
                                         canvas.width,
                                         canvas.height);

        }

        Keys.onSpacePressed: {
            var x_y = translate_mouse(mouse);
            this_player.request_split(x_y[0], x_y[1], "AUTH");
        }

        Keys.onPressed: {
            if (event.key === Qt.Key_W)
            {
                var x_y = translate_mouse(mouse);
                this_player.request_fire_food(x_y[0], x_y[1], "AUTH");
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
                canvas.this_player.request_coordinates(x_y[0], x_y[1], "AUTH");
            }
        }
    }
}
