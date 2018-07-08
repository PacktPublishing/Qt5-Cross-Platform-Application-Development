import QtQuick 2.11
import QtQuick.Controls 1.4

import Qt.WebSockets 1.0
import "qwebchannel.js" as WebChannel


ApplicationWindow {
    id: window
    title: "eatem"
    visible: true

    property var webchannel

    // FIXME: break this functionality down a layer, not at the window level
    // Game objects
    property var feed
    property var players
    property var viruses
    property var this_player
    property string authentication

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

                    window.feed = channel.objects.feed;
                    window.viruses = channel.objects.viruses;
                    window.this_player = channel.objects.this_player;
                    window.players = channel.objects.players;
                });
                break;
            }
        }
    }

    Canvas {
        id: canvas
        anchors.fill: parent
        focus: true
        contextType: "2d"
        property color fill_style: 'white'
        property var context

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

            // Set the fill style to clear clear background
            context.fillStyle = fill_style;

            // Clear the background
            context.clearRect(0, 0, canvas.width, canvas.height);

            // draw the food
            draw_food();

            // and draw the players
            draw_players();
        }

        function draw_food()
        {
            for (var i = 0; i < window.feed.length / 2; i++)
            {
                var food = window.feed[i];
                if (!food.enabled)
                    continue;
                context.beginPath();
                context.fillStyle = food.hue;
                context.arc(food.x, food.y, food.radius, 0, 2*Math.PI);
                context.fill();
            }
        }

        function draw_players()
        {

            for (var z=0; z < window.players.length; z++)
            {
                var player = window.players[z];
                context.fillStyle = player.hue;
                for (var cell_number=0; cell_number < player.cells.length; cell_number++)
                {
                    var cell = player.cells[cell_number];
                    context.beginPath();
                    context.arc(cell.x, cell.y, cell.radius, 0, 2*Math.PI);
                    context.fill();
                }
            }
        }

        Keys.onSpacePressed: {
            window.this_player.request_split(mouse.mouseX, mouse.mouseY, window.authentication);
        }

        MouseArea {
            id: mouse
            anchors.fill: parent
            hoverEnabled: true
        }

        Timer {
            id: lineTimer
            interval: 10
            repeat: true
            running: true
            onTriggered: {
                window.this_player.request_coordinates(mouse.mouseX, mouse.mouseY, window.authentication);
                canvas.requestPaint();
            }
        }
    }
}
