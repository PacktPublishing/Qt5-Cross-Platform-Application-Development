import QtQuick 2.11
import QtQuick.Controls 1.4

// Get key presses
// http://doc.qt.io/qt-5/qtquick-usecase-userinput.html#keyboard-and-button-events

// http://doc.qt.io/qt-5/qml-qtquick-keys.html#downPressed-signal

ApplicationWindow {
    id: window
    title: qsTr("eatem")
    visible: true

    Canvas {
        id: canvas
        anchors.fill: parent
        focus: true
        contextType: "2d"
        property int color: Math.round(Math.random() * 360)
        property color fill_style: 'white'
        property bool intialized: false
        property var context

        onPaint: {
            context = getContext("2d");
            // clear background
            context.fillStyle = fill_style;
            context.clearRect(0, 0, canvas.width, canvas.height);
            game_loop();
        }

        function game_loop()
        {
            requestAnimationFrame(game_loop);
            context.fillStyle = fill_style;
            context.clearRect(0, 0, canvas.width, canvas.height);
            draw_food();
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
                    // context.stroke();
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
                var mouse_x = mouse.mouseX;
                var mouse_y = mouse.mouseY
                this_player.request_coordinates(mouse_x, mouse_y);
                canvas.requestPaint()
            }
        }


    }

    /*
    InputPanel {
        id: inputPanel
        z: 99
        x: 0
        y: window.height
        width: window.width

        states: State {
            name: "visible"
            when: inputPanel.active
            PropertyChanges {
                target: inputPanel
                y: window.height - inputPanel.height
            }
        }
        transitions: Transition {
            from: ""
            to: "visible"
            reversible: true
            ParallelAnimation {
                NumberAnimation {
                    properties: "y"
                    duration: 250
                    easing.type: Easing.InOutQuad
                }
            }
        }
    }
    */
}
