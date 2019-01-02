import QtQuick 2.5
import QtQuick.Controls 2.2
import QtQuick.Controls.Styles 1.1
import Radius 1.0
import "../helper.js" as Helper
import "../components"

Rectangle {
    id: self
    color: "#333333"

    property string radius_uid: ""
    signal selectedEntry( string capture_uid )
    signal close()

    // Defines my data model
    ListModel {
        id: entries
        dynamicRoles: true

        Component.onCompleted: {
            entries.clear()
            ApiInterface.Capture.list( self.radius_uid,
                                      function (capture) {
                                          console.log("Captures: "+ capture.captures.length )
                                          for ( var i = 0; i < capture.captures.length; i++ )
                                          {
                                              var cap = capture.captures[i]
                                              var dur = Math.round( (cap.finish_ts - cap.start_ts) / 1000.0 )
                                              if ( dur < 1 )
                                                  dur = 1;

                                              //Add in a new list entry
                                              entries.append( { name: cap.name,
                                                                duration: Helper.formatTime( dur, true ),
                                                                distance: Helper.distanceToUnits( cap.distance, true ),
                                                                avg_speed: Helper.speedToUnits( cap.distance / dur, true ),
                                                                uid: cap.uid,
                                                                date: Helper.epochToDateTime(cap.start_ts / 1000.0),
                                                              } )
                                          }
                                      },
                                      function (err, timeout){
                                          console.log("Invalid response: "+ err.reason +" Timeout? "+ timeout)
                                          self.close()
                                      })
        }
    }

    ProfileMedallion {
        id: profileMedallion
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        //visible: false
        anchors.margins: parent.width * 0.025
        width: parent.width * 0.2
        height: parent.width * 0.2

        onClicked: function(uid) {
        }
    }

    // Show the list ivew
    ListView {
        anchors.top: profileMedallion.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: profileMedallion.anchors.margins
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

                Text {
                    id: entryDateTime
                    anchors.margins: 3
                    anchors.top: parent.top
                    anchors.left: parent.left
                    text: date
                    color: "white"
                    font.pixelSize: parent.height * 0.2
                }

                Image {
                    id: entryActivity
                    width: parent.height * 0.6
                    height: parent.height * 0.6
                    anchors.top: entryDateTime.bottom
                    anchors.horizontalCenter: entryDateTime.horizontalCenter
                    source: "../resources/activity/bicycle.png"
                }

                Text {
                    id: entryDistance
                    anchors.left: entryDateTime.right
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.leftMargin: 12
                    text: distance
                    color: "white"
                }

                Text {
                    id: entryDuration
                    anchors.left: entryDistance.right
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.leftMargin: 12
                    text: duration
                    color: "white"
                }

                Text {
                    id: entryAvgSpeed
                    anchors.left: entryDuration.right
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.leftMargin: 12
                    text: avg_speed
                    color: "white"
                }

                Rectangle {
                    id: lineBreak
                    color: "white"
                    height: parent.height * 0.02
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: self.selectedEntry( uid )
                }
            }
        }
    }
}
