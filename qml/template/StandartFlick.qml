import QtQuick 2.0
import QtQuick.Controls 2.4

Flickable{
    id:flick
    //anchors.fill:parent
    width: parent.width
    height: parent.height
    clip: true
    ScrollBar.horizontal: ScrollBar {
            parent: flick
            anchors.top: flick.bottom
            anchors.left: flick.left
            anchors.right: flick.right
            orientation: Qt.Horizontal
            policy: ScrollBar.AlwaysOn
            visible: flick.height<flick.contentHeight
        }

}
