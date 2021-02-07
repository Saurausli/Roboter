import QtQuick 2.10
import QtQuick.Controls 2.4
import QtQuick.Window 2.10
Window {
    id:window
    width: 800
    height:600
    visible: true
    title: qsTr("Console")
    Row{
        id:buttonRow
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        anchors.margins:10
        spacing: 10
        Button{
            text: "try"
            onClicked: {
                    Backend.tryCommand(textInput.getText())

            }
        }
        Button{
            text: "stop"
            onClicked: {
                    Backend.stopJoints()
                }
        }
        Button{
            text: "loop"
            onClicked: {
                    stopButton.visible=true;
                    Backend.loopCommand(textInput.editor.text)
                }
        }
        Button{
            id:stopButton
            visible: false
            text: "stop Loop"
            onClicked: {
                Backend.stopLoop()
            }
        }
    }
    Editor {
        id: textInput
        anchors.top: buttonRow.bottom
        anchors.bottom: respondsButtons.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 20
        //focus: true

    }
    Row{
        id:respondsButtons
        anchors.margins:5
        spacing: 10
        height: 20
        anchors.bottom: textOutput.top
        anchors.left: parent.left
        Button{
            text: "clear"
            onClicked: {
                    textOutput.text=""

                }
            font.pixelSize: 15
            height: 20
            width:implicitWidth
        }

    }
    Text {
        id: textOutput
        height: 150
        text: qsTr("")
        font.pixelSize: 15
        anchors.margins: 20
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

    }
    Connections{
        target: Backend
        onNewRespond:{
            textOutput.text=output+'\n'+textOutput.text
        }
        onNewRunningProgramm:{
            runningCode.editor.text=programm
        }
    }
}


