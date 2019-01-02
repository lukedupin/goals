import QtQuick 2.12

Rectangle {  //to be used inside MapComponent only
    id: self
    radius: self.width / 2
    border.width: self.width * 0.05
    border.color: "#22dbb5"
    smooth: true

    signal clicked()

    function setDistance( meters )
    {
        medallionDistance.text = (meters / 1609.34).toFixed(1)
    }

    function setSpeed( meters_per_second )
    {
        medallionSpeed.text = (meters_per_second * 2.23694).toFixed(1)
    }

    function setDuration( ms )
    {
        var seconds = ms / 1000
        var min = Math.floor( seconds / 60 ) % 60
        if ( min < 10 )
            min = "0"+ min
        var sec = Math.floor( seconds ) % 60
        if ( sec < 10 )
            sec = "0"+ sec

        //Write the time
        medallionDuration.text = Math.floor( seconds / 3600 ) +":"+ min
        medallionSeconds.text =  sec
    }

    //*** Speed related stuff

    Text {
        id: medallionSpeed
        text: qsTr("12.6")
        font.bold: true
        font.pixelSize: parent.width * 0.2
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: parent.height * 0.1
        visible: false
    }

    Text {
        text: qsTr("mph")
        font.pixelSize: parent.width * 0.073
        anchors.top: medallionSpeed.baseline
        anchors.horizontalCenter: parent.horizontalCenter
        visible: false
    }


    //*** Distance related stuff

    Text {
        id: medallionDistance
        text: qsTr("12.6")
        font.bold: true
        font.pixelSize: parent.width * 0.2
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: parent.height * 0.1
    }

    Text {
        text: qsTr("distance")
        font.pixelSize: parent.width * 0.073
        anchors.top: medallionSpeed.baseline
        anchors.horizontalCenter: parent.horizontalCenter
    }


    //** Activity and line break

    Rectangle {
        id: lineBreak
        color: "black"
        height: parent.height * 0.02
        //anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: medallionActivity.right
        anchors.leftMargin: parent.width * 0.025
        anchors.right: parent.right
        anchors.rightMargin: parent.width * 0.15
    }

    Image {
        id: medallionActivity
        width: parent.width * 0.2
        height: parent.height * 0.2
        anchors.verticalCenter: parent.verticalCenter
        anchors.bottomMargin: 4
        anchors.left: parent.left
        anchors.leftMargin: parent.width * 0.1
        source: "../resources/activity/bicycle.png"
    }


    //** Duration

    Item {
        id: containerDuration
        anchors.top: lineBreak.bottom
        anchors.topMargin: parent.height * 0.05
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width * 0.5
        //anchors.bottomMargin: parent.height * 0.1

        Text {
            id: medallionDuration
            text: qsTr("1:37")

            anchors.left: parent.left
            //anchors.verticalCenter: parent.verticalCenter
            font.bold: true
            font.pixelSize: self.width * 0.2
        }

        Text {
            id: medallionSeconds
            text: qsTr("54")

            anchors.left: medallionDuration.right
            anchors.baseline: medallionDuration.baseline
            font.pixelSize: self.width * 0.15
        }

        Text {
            text: qsTr("duration")
            font.pixelSize: self.width * 0.073
            anchors.top: medallionDuration.baseline
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: clicked()
    }
}
