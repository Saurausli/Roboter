import QtQuick 2.10
import QtQuick.Controls 2.4
import QtQuick.Window 2.10
Window {
    id:window
    width: 800
    height:600
    visible: true
    title: qsTr("Console")
    Component.onCompleted: {
        textInput.editor.text=Backend.loadFile();
    }

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

        editor.text: ""
        editor.font.pixelSize: 15
        editor.focus: true
        anchors.top: buttonRow.bottom
        anchors.bottom: respondsButtons.top
        anchors.left: parent.left
        anchors.right: parent.horizontalCenter
        anchors.margins: 20
        live:false
    }
    Editor {
        id: runningCode

        editor.text: ""
        editor.font.pixelSize: 15
        focus: true
        editor.focus: false
        anchors.margins: 20
        anchors.top: buttonRow.bottom
        anchors.bottom: respondsButtons.top
        anchors.right: parent.right
        anchors.left: parent.horizontalCenter
        live: true
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
        height: 250
        text: qsTr("")
        font.pixelSize: 15
        anchors.margins: 20
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

    }
    Connections{
        target: Backend
        onErrorOccured:{
            var err=[]
            textOutput.text=""
            err=Backend.getErrorMessageVec();
            for(var i=0;i<err.length;i++){
                textOutput.text=err[i]+'\n'+textOutput.text
            }
        }
        onNewRunningProgramm:{
            runningCode.editor.text=programm
        }
    }
}


