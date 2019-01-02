import QtQuick 2.12
import QtQuick.Controls 2.4
import QtLocation 5.6
import QtPositioning 5.5
import Radius 1.0
import "helper.js" as Helper

ApplicationWindow {
    id: appWindow
    property variant map
    property variant minimap
    property variant parameters

    //defaults
    property bool isIos: false

    Component.onCompleted: {
        isIos = (Platform.getDeviceType() === PlatformType.IOS)
    }

    title: qsTr("Goals")
    height: 640
    width: 180
    visible: true

    // Defines my data model
    ListModel {
         id: entries
         dynamicRoles: true

         Component.onCompleted: {
             entries.clear()
             var goals = SharedState.getStorage().goals
             for ( var i = 0; i < goals.length; i++ )
                 entries.append(goals[i])
         }
     }

    // Show the list ivew
    ListView {
        anchors.fill: parent
        clip: true

        ScrollBar.vertical: ScrollBar {
            policy: ScrollBar.AlwaysOn
        }

        model: entries
        delegate:  Component {
            id: entryDelegate
            Item {
            //Rectangle {
                id: wrapper
                anchors.margins: 3
                width: parent.width
                height: 40
                //color: "#333333"

                Label {
                    id: goal
                    anchors.margins: 3
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right
                    text: goal
                    color: "white"
                    font.pixelSize: parent.height * 0.2
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: self.selectedEntry( uid )
                }
            }
        }
    }
}
