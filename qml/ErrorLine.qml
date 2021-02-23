import QtQuick 2.0

Rectangle {
    id: element
    height: 25
    property int errorLine
    property string errorText
    signal clicked
    color: mouseArea.pressed ? "#265f99":"transparent"
    Row{
        id:textrow
        anchors.leftMargin: 5
        anchors.bottomMargin: 2
        spacing: 10
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: bottomLine.bottom
        Rectangle{
            color: "#d74044"
            width:  textrow.height*0.8
            height: width
            anchors.verticalCenter: parent.verticalCenter
            radius: height/2
            //anchors.verticalCenter: parent.verticalCenter
            //anchors.verticalCenter: parent.verticalCenter
            Text {
                text: qsTr("!")
                color: "white"
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: parent.height*0.8
            }
        }
        Text {
            color: "white"
            text: errorText
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 0
            font.pixelSize: textrow.height*0.8
            //anchors.verticalCenter: parent.verticalCenter
        }

    }
    MouseArea{
        id:mouseArea
        anchors.leftMargin: 0
        anchors.fill: parent
        onDoubleClicked: {
            textInput.showLine(errorLine)
            clicked

        }
    }
    Rectangle{
        id:bottomLine
        height: 1
        color: "white"
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
    }
}

/*##^##
Designer {
    D{i:4;anchors_height:23}
}
##^##*/
