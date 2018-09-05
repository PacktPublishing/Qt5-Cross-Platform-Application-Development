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
    property string authentication;

    WebSocket {
        id: socket
        active: true
        url: "ws://localhost:5555"

        property var send: function(arg) {
            sendTextMessage(arg);
        }

        onTextMessageReceived: {
            onmessage({data: message});
        }

        property var onmessage

        function set_auth_code(new_auth)
        {
            window.authentication = new_auth;

        }

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
                var webchannel = new WebChannel.QWebChannel(socket, set_auth_code, function(channel) {
                    var game_interface = channel.objects.interface;
                    window.game_interface = game_interface;
                    canvas.feed = game_interface.food;

                    var hue_num;

                    for (var i = 0; i < canvas.feed.length; i++){
                        hue_num = Math.round(Math.random() * 360)
                        canvas.feed[i].hue = 'hsl(' + hue_num + ', 100%, 50%)';
                    }

                    canvas.players = game_interface.players;
                    for (i = 0; i < canvas.players.length; i++){
                        hue_num = Math.round(Math.random() * 360)
                        canvas.players[i].hue = 'hsl(' + hue_num + ', 100%, 50%)';
                    }

                    canvas.viruses = game_interface.viruses;
                    game_interface.new_virus.connect(function(new_virus){
                        canvas.viruses.push(new_virus);
                    });

                    game_interface.new_player.connect(function(new_player){
                        canvas.players.push(new_player);
                    });

                    game_interface.new_food.connect(function(new_food){
                        canvas.feed.push(new_food);
                    });

                    game_interface.get_player(authentication, function(this_player){
                        canvas.this_player = this_player;
                        canvas.running = true;
                    });

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
        property color clear_color: 'white'
        property var context
        property var feed
        property var players
        property var viruses
        property var this_player
        property int gridSize: 30
        property string authentication
        property bool running: false

        onRunningChanged:
        {
            game_loop();
            lineTimer.start();
        }

        onPaint: {
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

            App.draw_objects(context,
                                         feed,
                                         players,
                                         viruses,
                                         this_player,
                                         canvas.width,
                                         canvas.height);
        }

        Keys.onSpacePressed: {
            if (!running)
                return;

            var x_y = translate_mouse(mouse);
            this_player.request_split(x_y[0], x_y[1], window.authentication);
        }

        Keys.onPressed: {
            if (!running)
                return;

            if (event.key === Qt.Key_W)
            {
                var x_y = translate_mouse(mouse);
                this_player.request_fire_food(x_y[0], x_y[1], window.authentication);
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
            onTriggered: {
                var x_y = canvas.translate_mouse(mouse);
                canvas.this_player.request_coordinates(x_y[0], x_y[1], window.authentication);
            }
        }
    }
}
