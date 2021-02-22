import QtQuick 2.10
import QtQuick.Controls 2.4
import QtQuick.Window 2.10
import "template/"
Window {
    id:window
    width: 800
    height:600
    visible: true
    title: qsTr("Console")
    Component.onCompleted: {
        textInput.setText(Backend.loadFile());
    }
    color: "#333333"
    Row{
        id:buttonRow
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        anchors.margins:10
        spacing: 10
        DarkButton{
            text: "try"
            onClicked: {
                    textInput.programmRunning=true
                    Backend.tryProgram(textInput.getProgramm())
                    //textInput.programmRunning=true
            }
            font.pixelSize: 25
        }
        DarkButton{
            text: "stop"
            onClicked: {
                    //Backend.stopJoints()
                    textInput.programmRunning=false
                }
            font.pixelSize: 25
        }
        DarkButton{
            text: "loop"
            onClicked: {
                    stopButton.visible=true;
                    Backend.loopCommand(textInput.editor.text)
                    textInput.programmRunning=true
                }
            font.pixelSize: 25
        }
        DarkButton{
            id:stopButton
            visible: false
            text: "stop Loop"
            onClicked: {
                Backend.stopLoop()
                textInput.programmRunning=false
            }
            font.pixelSize: 25
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
    Rectangle{
        id:respondsButtons
        height: 20
        anchors.bottom: flick.top
        anchors.left: parent.left
        anchors.right: parent.right
        color: "#232323"
        Row{
            anchors.fill: parent
            spacing: 10
            layoutDirection : Qt.RightToLeft
            TabBar{
                TabButton{
                    id:problems
                    text: "problems"

                }
                TabButton{
                    id:general
                    text: "general"

                }
            }
            DarkButton{
                text: "hide"
                onClicked: {
                    if(text=="hide"){
                        flick.height=0
                        text="show"
                    }
                    else{
                        flick.height=150
                        text="hide"
                    }


                    }
                 //Material.foreground: "white"
                font.pixelSize: 15
                height: 20
                width: 50
            }

            Rectangle{
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                width:2
                color: "white"
            }

            DarkButton{
                text: "clear"
                onClicked: {
                        textOutput.text=""

                    }
                font.pixelSize: 15
                height: 20
                width: 100
            }

        }
    }


    Flickable{
        id:flick
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 20
        height: 150
        clip: true

        contentHeight: textOutput.height>150 ? textOutput.height:150
        ScrollBar.horizontal: ScrollBar {
                parent: flick.parent
                anchors.top: flick.bottom
                anchors.left: flick.left
                anchors.right: flick.right
                orientation: Qt.Horizontal
                policy: ScrollBar.AlwaysOn
                visible: flick.width<textOutput.width
            }
        ErrorLine{
            visible: problems.checked
        }

        Text {
            id: textOutput
            clip: true
            visible: general.checked
            text: qsTr("")
            font.pixelSize: 15
            color: "white"
            Connections{
                target: Backend
                onNewOutput:{
                    textOutput.text+="\n"+output
                    if(textOutput.height>flick.height){
                        flick.contentY=textOutput.height-flick.height;
                    }
                }
            }
        }
    }


    Connections{
        target: Backend
        onErrorOccured:{
            textInput.programmRunning=false
        }
        onProgrammFinished:{
            textInput.programmRunning=false
        }
    }
}


