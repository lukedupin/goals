import QtQuick 2.11
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.4
import QtQuick.Controls.Styles 1.1
import Radius 1.0

Rectangle {
    id: signup
    color: "#444444"

    signal closeSignup(variant usr)

    /*
    ColumnLayout {
        id:parent
        spacing: 2
        anchors.fill: parent
        */

        Label {
            id: usernameLabel
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            text: qsTr("Username")
            color: "white"
            //Layout.fillWidth: true
            //Layout.alignment: Qt.AlignHCenter
            wrapMode: Text.WordWrap
            textFormat: Text.RichText
            anchors.leftMargin: 10
            anchors.rightMargin: 10
            anchors.bottomMargin: 10
        }

        TextField {
            id: username
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: usernameLabel.bottom
            placeholderText: ""
            //Layout.margins: 10
            //Layout.fillWidth: true
            //width: parent.width * 0.9
            //Layout.alignment: Qt.AlignHCenter
            anchors.margins: 10
            background: Rectangle {
                id: usernameRect
                radius: 5
                color: "white"
                //implicitWidth: 100
                implicitHeight: 24
                border.color: "#fd8402"
                border.width: 1
            }

            onEditingFinished: {
                ApiInterface.User.is_unique( username.text, null,
                                             function (unique) {
                                                 usernameRect.border.width = 2
                                                 if ( unique.name )
                                                 {
                                                     usernameRect.border.color = "green"
                                                     errorText.text = ""
                                                 }
                                                 else
                                                 {
                                                     usernameRect.border.color = "red"
                                                     errorText.text = qsTr("Name must be unique")
                                                 }
                                             })
            }
        }


        Label {
            id: realLabel
            anchors.top: username.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            text: qsTr("Human name")
            color: "white"
            //Layout.fillWidth: true
            //Layout.alignment: Qt.AlignHCenter
            wrapMode: Text.WordWrap
            textFormat: Text.RichText
            anchors.margins: 10
        }

        TextField {
            id: realName
            anchors.top: realLabel.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            placeholderText: ""
            //width: parent.width * 0.9
            //Layout.alignment: Qt.AlignHCenter
            anchors.margins: 10
            background: Rectangle {
                id: realNameBorder
                radius: 5
                color: "white"
                //implicitWidth: 100
                implicitHeight: 24
                border.color: "#fd8402"
                border.width: 1
            }

            onEditingFinished: {
                realNameBorder.border.width = 2
                realNameBorder.border.color = realName.text != ""? "green": "red"
            }
        }


        Label {
            id: emailLabel
            anchors.top: realName.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            text: qsTr("Email")
            color: "white"
            //Layout.fillWidth: true
            //Layout.alignment: Qt.AlignHCenter
            wrapMode: Text.WordWrap
            textFormat: Text.RichText
            anchors.margins: 10
        }

        TextField {
            id: email
            anchors.top: emailLabel.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            placeholderText: ""
            //width: parent.width * 0.9
            //Layout.alignment: Qt.AlignHCenter
            anchors.margins: 10
            background: Rectangle {
                id: emailRect
                radius: 5
                color: "white"
                //implicitWidth: 100
                implicitHeight: 24
                border.color: "#fd8402"
                border.width: 1
            }

            onEditingFinished: {
                ApiInterface.User.is_unique( null, email.text,
                                             function (unique) {
                                                 emailRect.border.width = 2
                                                 if ( unique.email )
                                                 {
                                                     emailRect.border.color = "green"
                                                     errorText.text = ""
                                                 }
                                                 else
                                                 {
                                                     emailRect.border.color = "red"
                                                     errorText.text = qsTr("Email must be unique")
                                                 }
                                             })
            }
        }

        Label {
            id: errorText
            text: qsTr("")
            anchors.top: email.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            //Layout.fillWidth: true
            //Layout.alignment: Qt.AlignHCenter
            wrapMode: Text.WordWrap
            textFormat: Text.RichText
            color: "red"
            anchors.margins: 10
        }

        Button {
            id: button
            anchors.top: errorText.bottom
            //anchors.left: parent.left
            //anchors.right: parent.right
            anchors.horizontalCenter: parent.horizontalCenter
            //anchors.margins: 10
            text: qsTr("OK")
            Layout.alignment: Qt.AlignHCenter

            onClicked: {
                ApiInterface.User.create( username.text, email.text, realName.text, ["Running", "Hiking"],
                                          47, -116,
                                          function (usr) {
                                              signup.closeSignup( usr )
                                          },
                                          function (err) {
                                              errorText.text = err.reason
                                          })
            }
        }
    //}
}
