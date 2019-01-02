import QtQuick 2.5
import QtQuick.Controls 1.4
import QtLocation 5.6
import QtPositioning 5.5
import Radius 1.0

Image
{
    //Signal actions
    signal captureActivity( bool capture )
    signal followSelected()

    id: capture
    state: "idle"
    //source: "../resources/capture_button_idle.png"

    property bool capturing: false

    MouseArea  {
        anchors.fill: parent
        hoverEnabled : false

        onPressed : {
            switch ( capture.state )
            {
                default:
                case "idle":
                    followSelected()
                    if ( capture.capturing )
                        capture.state = "capturing"
                    else
                        capture.state = "following"
                    break;

                case "following":
                    capture.capturing = true
                    captureActivity( true )
                    capture.state = "capturing"
                    break;

                case "capturing":
                    capture.capturing = false
                    captureActivity( false )
                    capture.state = "following"
                    break;
            }
        }
    }

    function stopFollowing()
    {
        capture.state = "idle"
    }

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
}
