import QtQuick 2.11
import QtQuick.Controls 1.4


ApplicationWindow {
    id: window
    title: "eatem"
    visible: true

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
            for (var i = 0; i < feed.length / 2; i++)
            {
                var food = feed[i];
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

            for (var z=0; z < players.length; z++)
            {
                var player = players[z];
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
            this_player.request_split(mouse.mouseX, mouse.mouseY);
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
                this_player.request_coordinates(mouse.mouseX, mouse.mouseY);
                canvas.requestPaint();
            }
        }
    }
}
