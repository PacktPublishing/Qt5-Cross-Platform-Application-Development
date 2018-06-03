import QtQuick 2.11
import QtQuick.Controls 1.4

// Get key presses
// http://doc.qt.io/qt-5/qtquick-usecase-userinput.html#keyboard-and-button-events

// http://doc.qt.io/qt-5/qml-qtquick-keys.html#downPressed-signal

ApplicationWindow {
    id: window
    title: qsTr("eatem")
    visible: true

    onHeightChanged: {
        if (window.height === 0)
            return;
        if (canvas.foods.length === 100)
            return;
        for (var i = 0; i < 100; i++)
        {
            var x = Math.round(Math.random()* window.width);
            var y = Math.round(Math.random()* window.height);
            var hue_num = Math.round(Math.random() * 360)
            var hue_str = 'hsl(' + hue_num + ', 100%, 50%)'
            canvas.foods.push([x, y, hue_str])
        }
    }
    Canvas {
        id: canvas
        anchors.fill: parent
        focus: true
        contextType: "2d"
        property int color: Math.round(Math.random() * 360)
        property color fill_style: 'white'
        property var foods: []
        property string player_color_string: 'hsl(' + color + ', 100%, 50%)'

        onPaint: {
            var context = canvas.getContext("2d");

            // clear background
            context.fillStyle = fill_style;
            context.clearRect(0, 0, canvas.width, canvas.height)

            context.fillStyle = player_color_string
            context.beginPath();
            context.arc(this_player.x, this_player.y, this_player.radius, 0, 2*Math.PI);
            // context.stroke();
            context.fill();
            for (var i = 0; i < foods.length; i++)
            {
                context.beginPath();
                var x = foods[i][0];
                var y = foods[i][1];
                var hue = foods[i][2];
                context.fillStyle = hue;
                context.arc(x, y, 5, 0, 2*Math.PI)
                context.fill();
            }
            var eat_me = []
            for (var i = 0; i < foods.length; i++)
            {
                if (eat_food(foods[i]))
                {
                    eat_me.push(i)
                }
            }
            for (i = eat_me.length -1; i >=0; i--)
                foods.splice(eat_me[i], 1);
            // mass += eat_me.length
        }

        function eat_food(food)
        {
            var diff_x = food[0] - this_player.x
            var diff_y = food[1] - this_player.y
            var distance = Math.sqrt(Math.pow(diff_x, 2)+ Math.pow(diff_y, 2))
            return distance <= this_player.radius + 5


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
