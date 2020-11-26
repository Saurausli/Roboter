import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
Window {
    id:window
    width: 800
    height:600
    visible: true
    title: qsTr("Console")
    Row{
        id:buttonRow
        anchors.margins:10
        spacing: 10
        anchors.top: parent.top
        anchors.left: parent.left
        Button{
            text: "try"
            onClicked: {
                    Backend.tryCommand(textInput.editor.text)
                }
        }
        Button{
            text: "stop"
            onClicked: {
                    Backend.stop()
                }
        }
        Button{
            text: "loop"
            onClicked: {
                    Backend.loopCommand(textInput.text)
                }
        }
    }

    Editor {
        id: textInput

        editor.text: "Text Input"
        editor.font.pixelSize: 20
        editor.focus: true
        anchors.top: buttonRow.bottom
        anchors.bottom: textOutput.top
        anchors.left: parent.left
        anchors.right: parent.horizontalCenter
        anchors.margins: 20
    }
    Text {
        id: runningCode

        text: "Text Input"
        font.pixelSize: 15
        focus: true
        anchors.margins: 20
        anchors.top: buttonRow.bottom
        anchors.bottom: textOutput.top
        anchors.right: parent.right
        anchors.left: parent.horizontalCenter
    }
    Text {
        id: textOutput
        height: 250
        text: qsTr("text")
        font.pixelSize: 15
        anchors.margins: 20
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        Connections{
            target: Backend
            function onNewRespond(output){
                textOutput.text=output
            }
        }
    }
}


