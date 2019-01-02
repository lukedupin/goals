import QtQuick 2.11
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.4
import QtQuick.Controls.Styles 1.1

Item {
    id: root

    property alias title: titleUi.text
    property alias text: inputUi.text
    property variant backPage

    signal closeForm(variant text, variant backPage)

    ColumnLayout {
        id:parent
        spacing: 20
        anchors.fill: parent

        Label {
            id: titleUi
            text: qsTr("message")
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter
            wrapMode: Text.WordWrap
            textFormat: Text.RichText
        }

        TextField {
            id: inputUi
            placeholderText: "TextField with background"
            width: parent.width * 0.9
            text: "TextField with background"
            Layout.alignment: Qt.AlignHCenter
            background: Rectangle {
                radius: 5
                color: "white"
                implicitWidth: 100
                implicitHeight: 24
                border.color: "#fd8402"
                border.width: 1
            }
        }

        Button {
            id: button
            text: qsTr("OK")
            Layout.alignment: Qt.AlignHCenter

            onClicked: {
                console.log("BOBSSDFD: "+ inputUi.displayText)
                root.closeForm(inputUi.text, root.backPage)
            }
        }
    }
}
