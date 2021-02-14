import QtQuick 2.10
import QtQuick.Controls 2.13
Item{
    id:lineId
    x: 0;
    y: line*height
    property color color
    property color textColor
    property string text
    property int line

    Rectangle{
        rotation: -90
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        height: parent.width
        width: lineId.height
        gradient: Gradient {
                GradientStop { position: 0.3; color: "transparent" }
                GradientStop { position: 1.0; color: lineId.color }
            }

        }
    Text{
       id:text
       anchors.right: parent.right
       anchors.rightMargin: 5
       color: textColor
       text:lineId.text
       font: textEditInput.font
    }
}
