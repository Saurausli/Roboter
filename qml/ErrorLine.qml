import QtQuick 2.0

Item {
    anchors.left: parent.left
    anchors.right: parent.right
    height: 20
    Row{
        anchors.fill:parent.fill
        Rectangle{
            color: "red"
            height: 15
            width: height
            radius: height/2
            anchors.verticalCenter: parent.verticalCenter
            Text {
                text: qsTr("!")
                color: "white"
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }

    }
}
