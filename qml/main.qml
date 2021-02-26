import QtQuick 2.10
import QtQuick.Controls 2.4
import QtQuick.Window 2.10
//import QtQuick.Layouts 2.4
import "template/"
import QtQuick.Layouts 1.3
import "template"
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
        anchors.bottom: bottomBar.top
        anchors.left: parent.left
        anchors.right: parent.right
        color: "#232323"
        Row{
            anchors.fill: parent
            spacing: 10
            layoutDirection : Qt.RightToLeft
            DarkButton{
                text: "hide"
                onClicked: {
                    if(text=="hide"){
                        bottomBar.visible=false
                        text="show"
                    }
                    else{
                        bottomBar.visible=true
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



        StackLayout{
            id:bottomBar
            currentIndex: bottomBarTabBar.currentIndex
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: bottomBarTabBar.top
            anchors.margins: 20
            height: 250
            anchors.bottomMargin: 0
            property variant error: []
            property variant errorMessage: []
            Connections{
                target: Backend
                onErrorOccured:{
                    bottomBar.errorMessage=Backend.getErrorMessageVec()
                    bottomBar.error=Backend.getErrorLineVec();

                }
            }
            StandartFlick{
                id:generalFlick
                contentHeight: textOutput.height>bottomBar.height ? textOutput.height:bottomBar.height
                Text {
                    id: textOutput
                    clip: true
                    text: qsTr("")
                    font.pixelSize: 15
                    color: "white"
                    Connections{
                        target: Backend
                        onNewOutput:{
                            textOutput.text+="\n"+output
                            if(textOutput.height>generalFlick.height){
                                generalFlick.contentY=textOutput.height-generalFlick.height;
                            }

                        }
                    }
                }
            }
        StandartFlick{
            contentHeight: errorColmn.height>bottomBar.height ? errorColmn.height:bottomBar.height
            Column{
                id:errorColmn
                property int errorHeight: 25
                Repeater{
                    id:errorList
                    model:bottomBar.error.length

                    ErrorLine{
                        id:errorLine
                        //x:0
                        //y:height*index
                        height: errorColmn.errorHeight
                        width: bottomBar.width
                        errorLine: bottomBar.error[index]
                        errorText: bottomBar.errorMessage[index]
                    }
                }
                height: bottomBar.error.length*errorHeight
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
        TabBar{
            id:bottomBarTabBar
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            height: 20
            currentIndex: 0
            background: Rectangle {
                    color: "#232323"
                }
            DarkTabButton{
                id:general
                text: "general"
                height: 20
                anchors.top: parent.top
                width: 100
                //Component.on
            }
            DarkTabButton{
                id:problems
                text: "problems"
                height: 20
                anchors.top: parent.top
                width: 100
            }

        }

}
