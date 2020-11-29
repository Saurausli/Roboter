import QtQuick 2.10
import QtQuick.Controls 2.4

Flickable {
     id: editor

     width: 300; height: 200;
     contentWidth: te.paintedWidth
     contentHeight: te.paintedHeight
     clip: true
     property bool live: false
     property alias editor: te
     property int liveCommand: -1
     property variant error: []
     Connections{
         target: Backend
         onNewRunningCommand:{
             liveCommand=commandLine
         }
         onErrorOccured:{
             console.debug(line,error,error.length)
             error[error.length]=line
             var a=[];
             a=error
             error=a
             console.debug(line,error)
         }
         onNewRunningProgramm:{
             error=[]
         }
     }

    boundsMovement: Flickable.StopAtBounds
    ScrollBar.vertical: ScrollBar {
        visible: true
        anchors.right: parent.right
        anchors.top: parent.top
    }
     Rectangle {
         id: root
         height: te.implicitHeight<editor.height ? editor.height: te.implicitHeight
         width: editor.width
         color: "lightgrey"
         onHeightChanged: {
             num.text="1"
             for(var i=2;i<=height/fontMetrics.height;i++){
                 num.text=num.text+'\n'+i
             }
         }

         FontMetrics {
             id: fontMetrics
             font: te.font
         }

         Rectangle {
             x: 0; y: te.cursorRectangle.y
             height: fontMetrics.height
             width: root.width
             color: "#e7e7e7"
             visible: te.activeFocus
         }
         Rectangle {
             x: 0;
             y: liveCommand*fontMetrics.height
             height: fontMetrics.height
             width: root.width
             color: "limegreen"
             visible: live
         }
         Repeater{
             model: error
             Rectangle{
                 x: 0;
                 y: error[index]*fontMetrics.height
                 height: fontMetrics.height
                 width: root.width
                 color: "red"
                 visible: live
             }
         }

         Text{
             id: num
             anchors.left: parent.left
             anchors.top: parent.top
             anchors.leftMargin: 10
             font: te.font
             text: "0"

         }

         TextEdit {
             id: te
             anchors.leftMargin: 10
             anchors.left: num.right
             width: flick.width
             anchors.top: parent.top
             wrapMode: TextEdit.Wrap
             selectByMouse: true
            onCursorRectangleChanged: editor.ensureVisible(cursorRectangle)
         }
         MouseArea{
             anchors.fill: parent
             onClicked: {
                 te.focus=true
             }
         }
     }


     function ensureVisible(r)
     {
         if (contentX >= r.x)
             contentX = r.x;
         else if (contentX+width <= r.x+r.width)
             contentX = r.x+r.width-width;
         var yTemp
         yTemp=contentY
         if (yTemp >= r.y)
             yTemp = r.y;
         else if (yTemp+height <= r.y+r.height){
             yTemp = r.y+r.height-height
         }
         contentY=yTemp
          }
 }
