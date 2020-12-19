import QtQuick 2.10
import QtQuick.Controls 2.12

Flickable {
     id: editor

     width: 300; height: 200;
     contentWidth: te.paintedWidth
     contentHeight: te.paintedHeight
     clip: true

     property bool live: false
     property alias editor: te
     property int liveCommand: -1
     property variant errorLine: []
     property variant errorMessage: []
     property string text: ""
     onTextChanged: {
         if(live){
                //displayColorText()
             }

         }

     Connections{
         target: Backend
         onNewRunningCommand:{
             liveCommand=commandLine
         }
         onErrorOccured:{
             errorMessage=Backend.getErrorMessageVec();
             errorLine=Backend.getErrorLineVec();
         }
         onNewRunningProgramm:{
             //errorLine=[]
         }
     }
     Shortcut {
         sequence: StandardKey.Copy
         onActivated: te.copy()
     }
     Shortcut {
         sequence: StandardKey.Cut
         onActivated: {if(!te.readOnly) te.cut()}
     }
     Shortcut {
         sequence: StandardKey.Paste
         onActivated: {if(!te.readOnly) te.paste()}
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
         color: "#2e2f30"
         onHeightChanged: {
             num.text="1"
             for(var i=2;i<=height/fontMetrics.height;i++){
                 num.text=num.text+'\n'+i
             }
         }



        Rectangle{
            anchors.left: root.left
            anchors.top: root.top
            anchors.bottom: root.bottom
            width:(num.anchors.leftMargin*1.5)+num.implicitWidth
            color: "#404244"

        }
         FontMetrics {
             id: fontMetrics
             font: te.font
         }

         /*Rectangle {
             x: 0; y: te.cursorRectangle.y
             height: fontMetrics.height
             width: root.width
             color: "#2e2f30"
             visible: !live&&te.activeFocus
         }*/
         Rectangle {
             x: 0;
             y: liveCommand*fontMetrics.height
             height: fontMetrics.height
             width: root.width
             color: "#588a2c"
             visible: live
         }

         Repeater{
             model: errorLine.length
             Item{
                 x: 0;
                 y: errorLine[index]*fontMetrics.height
                 height: fontMetrics.height
                 width: root.width
                 Rectangle{
                     rotation: -90
                     anchors.horizontalCenter: parent.horizontalCenter
                     anchors.verticalCenter: parent.verticalCenter
                     height: root.width
                     width: fontMetrics.height
                     gradient: Gradient {
                             GradientStop { position: 0.3; color: "transparent" }
                             GradientStop { position: 1.0; color: "#473233" }
                         }

                     }
                 visible: live
                 Text{
                    anchors.right: parent.right
                    anchors.rightMargin: 5
                    color: "#e58076"
                    text:{
                        var t
                        if(errorMessage.length>index){
                            t= errorMessage[index].substring(errorMessage[index].indexOf("Error:")+7)
                            t= t.substring(0,t.indexOf(";"))
                        }
                        else{
                            return ""
                        }
                    }
                 }
             }
         }
         TextEdit {
             id: te
             anchors.leftMargin: 10
             anchors.left: num.right
             width: flick.width
             color:"#d6cf9a"
             anchors.top: parent.top
             renderType: Text.NativeRendering
             wrapMode: TextEdit.Wrap
             selectByKeyboard:true
             selectByMouse: true
             onSelectedTextChanged: {
                 console.debug(selectedText)
             }

             selectedTextColor: "#bec0c2"
             selectionColor: "#1d545c"
             mouseSelectionMode: TextEdit.SelectCharacters
             readOnly : live
             onCursorRectangleChanged: editor.ensureVisible(cursorRectangle)
             persistentSelection: true
         }
         Text{
             id: num
             anchors.left: parent.left
             anchors.top: parent.top
             anchors.leftMargin: 10
             font: te.font
             text: "0"
             color: "#bec0c2"

         }

         MouseArea{
             anchors.fill: parent
             onClicked: {
                 if(!te.focus){
                 te.focus=true
                 }
             }
         }
     }

    function displayColorText(){
        var keyword=[];
        keyword=Backend.getFuncitionKeyWords()
        te.textFormat=TextEdit.PlainText
        te.text=text
        te.text=" "+te.text+" "
        for(var i=0;i<keyword.length;i++){
            replaceString(keyword[i],"<font color=\"#45c6d6\">")
        }
        //te.text=te.text.replace(/\n/g,"<br>")
        var search=new RegExp("//", "g")
        te.text=te.text.replace(search,"<font color=\"#a8abb0\">//")

        var charIndex=0;
        charIndex =te.text.indexOf("//",charIndex);
        while(charIndex>-1){
            charIndex =te.text.indexOf("//",charIndex);

            var endIndex=te.text.indexOf("\n",charIndex)
            if(endIndex<0){
                endIndex=te.text.length-1
            }


            if(charIndex>-1){
                te.text= te.text.substring(0,endIndex)+"</font>"+te.text.substring(endIndex)
                charIndex=endIndex+6;
            }
        }
        te.text=te.text.replace(/\n/g,"<br>")
        //te.text=te.text.substring(1);
        te.textFormat=TextEdit.RichText

    }

    function replaceString(word,fontStartText){
        var search=new RegExp("\n"+word+"\n", "g")
        te.text=te.text.replace(search,"\n"+fontStartText+word+"</font>\n")
        search=new RegExp("\n"+word+" ", "g")
        te.text=te.text.replace(search,"\n"+fontStartText+word+"</font> ")
        search=new RegExp(" "+word+"\n", "g")
        te.text=te.text.replace(search," "+fontStartText+word+"</font>\n")
        search=new RegExp(" "+word+" ", "g")
        te.text=te.text.replace(search," "+fontStartText+word+"</font> ")
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
