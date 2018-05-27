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
        property int player_x: window.width / 2
        property int player_y: window.height / 2
        property int mass: 20
        property int radius: 4 + Math.sqrt(mass) * 6;
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
            context.arc(player_x, player_y, radius, 0, 2*Math.PI);
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
            mass += eat_me.length
        }

        function eat_food(food)
        {
            var diff_x = food[0] - player_x
            var diff_y = food[1] - player_y
            var distance = Math.sqrt(Math.pow(diff_x, 2)+ Math.pow(diff_y, 2))
            return distance <= radius + 5


        }

        MouseArea {
            id: mouse
            anchors.fill: parent
        }

        Timer {
            id: lineTimer
            interval: 10
            repeat: true
            running: true
            onTriggered: {
                var mouse_x = mouse.mouseX;
                var mouse_y = mouse.mouseY
                var player_x = canvas.player_x
                var player_y = canvas.player_y

                if (mouse_x === 0 && mouse_y === 0)
                    return;
                var target_x = player_x - mouse_x
                var target_y = player_y - mouse_y

                var dist = Math.sqrt(Math.pow(target_x, 2) + Math.pow(target_y, 2));
                // prevents jitter
                if (dist < 3)
                    return;
                var deg = Math.atan2(target_y, target_x);
                var slowDown = 1

                var deltaY = 3 * Math.sin(deg)/ slowDown;
                var deltaX = 3 * Math.cos(deg)/ slowDown;
                // console.log(target_x, target_y, player_x, player_y, deltaX, deltaY)
                // console.log(deltaX, deltaY)
                canvas.player_x -= deltaX
                canvas.player_y  -= deltaY
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
