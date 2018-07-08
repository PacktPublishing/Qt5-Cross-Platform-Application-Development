import QtQuick 2.11
import QtQuick.Controls 1.4
import GameInterfaces 1.0


ApplicationWindow {
    id: window
    title: "eatem"
    visible: true

    GameInterface {
        id: game_interface
        Component.onCompleted: {
            game_interface.set_game_size(1000, 1000);
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
        property var this_player: game_interface.this_player

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
            draw_grid();

            // draw the food
            draw_food();

            // and draw the players
            draw_players();

        }

        function translate(object)
        {
            var relative_x = object.x + (window.contentItem.width/2) - this_player.x;
            var relative_y = object.y + (window.contentItem.height/2) - this_player.y;

            // FIXME: figure out what this magic number should actually be
            var zoomed_radius = object.radius * this_player.zoom_factor;
            relative_x *= this_player.zoom_factor;
            relative_y *= this_player.zoom_factor;
            return [relative_x, relative_y, zoomed_radius];
        }

        function draw_grid()
        {
            var gridSize = 30;    // define the space between each line
            var width = window.contentItem.width;
            var height = window.contentItem.height
            var x = width/2 - this_player.x;  // x start point of the field
            var y = height/2 - this_player.y  // y start point of the field
            var absolute_x, absolute_y;
            // console.log()

            context.lineWidth = 1;
            context.beginPath();
            for(var i = 0; i * gridSize < height; i++) { // draw the horizontal lines
                absolute_x = x + this_player.x;
                if (absolute_x <= 0 || absolute_x > 1000)
                    continue;
               context.moveTo(x, i * gridSize + y);
               context.lineTo(x + width, i * gridSize + y);
            }
            for(i = 0; i * gridSize < width; i++) {  // draw the vertical lines
                absolute_y = y + this_player.y;
                if (absolute_y <= 0 || absolute_y > 1000)
                    continue;
               context.moveTo(i * gridSize + x,  y);
               context.lineTo(i * gridSize + x, y + height);
            }
            context.stroke();

        }

        function draw_food()
        {
            var x_y_radius, food, x, y;

            for (var i = 0; i < feed.length / 2; i++)
            {
                food = feed[i];
                if (!food.enabled)
                    continue;
                x_y_radius = translate(food)
                x = x_y_radius[0];
                y = x_y_radius[1];
                if (x > window.contentItem.width || x < 0)
                    continue;
                if (y > window.contentItem.height || y < 0)
                    continue;
                context.beginPath();
                context.fillStyle = food.hue;
                context.arc(x_y_radius[0],
                            x_y_radius[1],
                            x_y_radius[2], 0, 2*Math.PI);

                context.fill();
            }
        }

        function draw_players()
        {
            var x_y_radius, player, cell;

            for (var z=0; z < players.length; z++)
            {
                player = players[z];
                context.fillStyle = player.hue;
                for (var cell_number=0; cell_number < player.cells.length; cell_number++)
                {
                    cell = player.cells[cell_number];
                    x_y_radius = translate(cell);
                    context.beginPath();
                    context.arc(x_y_radius[0],
                                x_y_radius[1],
                                x_y_radius[2],
                                0, 2*Math.PI);

                    context.fill();
                }
            }
        }

        Keys.onSpacePressed: {
            // FIXME: this is wrong now
            var x_y = translate_mouse(mouse);
            this_player.request_split(x_y[0], x_y[1]);
        }

        MouseArea {
            id: mouse
            anchors.fill: parent
            hoverEnabled: true
        }

        function translate_mouse(mouse)
        {
            // NOTE: probably need to scale this as well
            return [mouse.mouseX - (window.contentItem.width/2 * this_player.zoom_factor) + this_player.x,
                    mouse.mouseY - (window.contentItem.height/2 * this_player.zoom_factor) + this_player.y];
        }

        Timer {
            id: lineTimer
            interval: 10
            repeat: true
            running: true
            onTriggered: {
                var x_y = canvas.translate_mouse(mouse);
                canvas.this_player.request_coordinates(x_y[0], x_y[1]);
                canvas.requestPaint();
            }
        }
    }
}
