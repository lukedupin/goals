import QtQuick 2.12
import QtQuick.Controls 2.4
import QtLocation 5.6
import QtPositioning 5.5
import Radius 1.0

Rectangle {
    id: titleBar

    signal backButton()

    Rectangle {
        id: backButton
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        width: 30
        color: "#FF0000"
        visible: isIos

        MouseArea {
            anchors.fill: parent
            onClicked: backButton()
        }
    }

    states: [
        State {
            name: "SHOW"
            PropertyChanges { target: titleBar; y: 0  }
        },
        State {
            name: "HIDE"
            PropertyChanges { target: titleBar; y: -height }
        }
    ]

    transitions: [
        Transition {
            from: '*'; to: '*'
            NumberAnimation { properties: 'y'; duration: 300; easing.type: Easing.InOutQuint }
        }
    ]
}
