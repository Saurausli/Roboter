import QtQuick 2.12
import QtQuick.Controls 2.12

TabButton {
    id: control
    text: qsTr("Button")

    contentItem: Text {
        text: control.text
        font: control.font
        color: control.down ? "#232323":"white"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
        implicitWidth: 100
        implicitHeight: 40
        color: control.down ? "white":"#232323"
    }
}
