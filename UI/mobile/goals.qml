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
                 entries.append({"goal": goals[i].goal, "count": goals[i].count,
                                 "idx": i, "user_read": false})
         }
     }

    //Done button =)
    Button {
        id: doneButton
        anchors.fill: parent
        height: 90
        width: 90
        anchors.margins: 20
        text: "Click here if\nI know I can do this!!!!"
        visible: false

        onClicked: {
            var ary = []
            for ( var i = 0; i < entries.count; i++ )
                ary.push( {"goal": entries.get(i)["goal"], "count": entries.get(i)["count"] })
            Mailbox.emitDoneQml({"goals": ary})
        }
    }

    //Add button
    Button {
        id: addGoal
        text: "Add Goal"
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        height: 30
        visible: !doneButton.visible

        onClicked: entries.append({"goal": "New goal", "idx": entries.count,
                                   "count": 0, "user_read": false})
    }

    // Show the list ivew
    ListView {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: addGoal.top
        clip: true
        visible: !doneButton.visible

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

                property bool editable: false

                Rectangle {
                    id: goalViewableParent
                    radius: 15
                    border.color: user_read? "green": "red"
                    border.width: 2

                    anchors.margins: 3
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right

                    height: parent.height

                    visible: !editable

                    Label {
                        id: goalViewable
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.margins: 4
                        text: goal
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            user_read = true
                            count++
                            for ( var i = 0; i < entries.count; i++ )
                            {
                                if ( !entries.get(i)["user_read"] )
                                    return;
                            }

                            doneButton.visible = true
                        }

                        onDoubleClicked: wrapper.editable = true
                    }
                }

                TextField {
                    id: goalEditable
                    anchors.margins: 3
                    anchors.left: parent.left
                    anchors.right: remove.left

                    height: parent.height

                    text: goal
                    visible: editable

                    onEditingFinished: {
                        goal = goalEditable.text
                        wrapper.editable = false
                    }
                }

                Button {
                    id: remove
                    anchors.right: parent.right
                    anchors.margins: 3

                    height: parent.height
                    width: 40

                    text: "Rm"
                    visible: editable
                    onClicked: {
                        entries.remove(idx)
                    }
                }
            }
        }
    }
}
