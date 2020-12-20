import QtQuick 2.10
import QtQuick.Controls 2.10
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
    color: "#333333"
    Rectangle{
        id:topRow
        color: "#232323"
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: 35+(buttonRow.anchors.margins*2)
        Row{
            id:buttonRow
            anchors.fill: parent
            anchors.margins:10
            spacing: 10
            Button{
                id:tryButton
                width: 40
                text: "try"
                focusPolicy: Qt.StrongFocus
                anchors.bottom: parent.bottom
                anchors.top: parent.top
                background: Rectangle{color:"#dbdbdb"}
                onClicked: {
                        Backend.tryCommand(textInput.getText())

                    }
            }
            Button{
                width: 40
                text: "stop"
                anchors.bottom: parent.bottom
                anchors.top: parent.top
                background: Rectangle{color:"#dbdbdb"}
                onClicked: {
                        Backend.stopJoints()
                    }
            }
            Button{
                width: 40
                text: "loop"
                anchors.bottom: parent.bottom
                anchors.top: parent.top
                background: Rectangle{color:"#dbdbdb"}
                onClicked: {
                        stopButton.visible=true;
                        Backend.loopCommand(textInput.getText())

                    }
            }
            Button{
                id:stopButton
                width: 70
                visible: false
                text: "stop Loop"
                background: Rectangle{color:"#dbdbdb"}
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                onClicked: {
                    Backend.stopLoop()
                }
            }
        }
    }

    Editor {
        id: textInput

        editor.text: ""
        editor.font.pixelSize: 15
        editor.focus: true
        anchors.top: topRow.bottom
        anchors.bottom: respondsButtons.top
        anchors.left: parent.left
        anchors.right: parent.horizontalCenter
        anchors.margins: 20
        live:false
    }
    Editor {
        id: runningCode

        //editor.text: ""
        editor.font.pixelSize: 15
        focus: true
        editor.focus: false
        anchors.margins: 20
        anchors.top: topRow.bottom
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
        height: 100
        color: "#cce7e7"
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
            runningCode.text=programm
        }
    }
}


