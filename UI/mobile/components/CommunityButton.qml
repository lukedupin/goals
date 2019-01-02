import QtQuick 2.5
import QtQuick.Controls 1.4
import QtLocation 5.6
import QtPositioning 5.5
import Radius 1.0

Rectangle
{
    id: social
    color: "orange"
    radius: 8
    //state: "idle"
    //source: "../resources/capture_button_idle.png"

    signal click()

    MouseArea  {
        anchors.fill: parent
        hoverEnabled : false

        onPressed : {
            click()
        }
    }

    /*
    states: [
        State {
            name: "idle"
            PropertyChanges { target: capture; source: "../resources/follow_me.png" }
        },
        State {
            name: "following"
            PropertyChanges { target: capture; source: "../resources/start_capture.png" }
        },
        State {
            name: "capturing"
            PropertyChanges { target: capture; source: "../resources/stop_capture.png" }
        }
    ]
    */
}
