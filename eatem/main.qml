import QtQuick 2.11
import QtQuick.Controls 1.4

import "../core/app.js" as App
import "../core/qwebchannel.js" as WebChannel
import Qt.WebSockets 1.0


ApplicationWindow {
    id: window
    title: "eatem"
    visible: true
    property var game_interface

    WebSocket {
        id: socket
        active: true
        url: "ws://localhost:5555"
         onStatusChanged: {
            switch (socket.status) {
            case WebSocket.Error:
                console.error("Error: " + socket.errorString);
                break;
            case WebSocket.Closed:
                console.error("Error: Socket at " + url + " closed.");
                break;
            case WebSocket.Open:
                //open the webchannel with the socket as transport
                new WebChannel.QWebChannel(socket, function(channel) {
                    window.channel = channel;
                    window.game_interface = channel.objects.interface;
                    canvas.feed = window.game_interface.food;
                    canvas.players = window.game_interface.players;
                    canvas.viruses = window.game_interface.viruses
                });
                canvas.game_loop();
                break;
            }
        }
    }


    Canvas {
        id: canvas
        anchors.fill: parent
        focus: true
        contextType: "2d"
        property color clear_color: 'white'
        property var context
        property var feed
        property var players
        property var viruses
        property var this_player
        property int gridSize: 30
        property string authentication

        Component.onCompleted:
        {
            context = getContext("2d");
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
