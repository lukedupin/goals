import QtQuick 2.12
import QtGraphicalEffects 1.12
import Radius 1.0

Rectangle {  //to be used inside MapComponent only
    id: self
    radius: self.width / 2
    color: "white"
    smooth: true

    property string loadingSource: "https://img-aws.ehowcdn.com/600x600p/photos.demandstudios.com/getty/article/129/142/75546255.jpg"
    property string _uid: "self"
    property variant _profile: ({})

    //Let the user know when we've been clicked on
    signal clicked(string uid)

    function setUid( uid )
    {
        _uid = uid

        //Load the source with a property that we already have
        if ( _profile.hasOwnProperty(uid))
        {
            profile.source = _profile[uid]
            return
        }

        //Conduct a server request for the profile
        profile.source = self.loadingSource
        ApiInterface.Radius.desc( uid, function (desc) {
            profile.source = desc.profile
        })
    }

    Image {
        id: profile
        width: parent.width * 0.95
        height: parent.height * 0.95
        anchors.centerIn: parent
        source: self.loadingSource

        fillMode: Image.PreserveAspectCrop
        layer.enabled: true
        layer.effect: OpacityMask {
            maskSource: self
        }
    }

    MouseArea
    {
        anchors.fill: parent
        onClicked: {
            console.log("Clicked on with: "+ self._uid)
            self.clicked(self._uid)
        }
    }
}
