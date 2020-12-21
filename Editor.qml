import QtQuick 2.10

import QtQuick.Controls 2.13
//import QtQml 2.13

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
     property string textProv
     onTextChanged: {
         if(live){
                displayColorText()
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
             textFormat:TextEdit.RichText
             property int lengthPrevious: 0
             onLengthChanged: {
                 if(length!=lengthPrevious){
                     lengthPrevious=length;
                     displayColorText();
                 }
             }
             onEditingFinished: {
                 displayColorText();
             }
             selectedTextColor: "#bec0c2"
             selectionColor: "#1d545c"
             mouseSelectionMode: TextEdit.SelectCharacters
             readOnly : live
             onCursorRectangleChanged: editor.ensureVisible(cursorRectangle)
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
    function getText(){
        var temp
        temp=te.getText(0,300)
        var replaceStringTemp=String.fromCharCode(8233)
        var search=new RegExp(replaceStringTemp, "g")
        temp=temp.replace(search,"\n")
        replaceStringTemp=String.fromCharCode(8232)
        search=new RegExp(replaceStringTemp, "g")
        temp=temp.replace(search,"\n")
        return temp
    }
    function setText(string){
        string="<pre>"+string+"</pre>"
        var cursorPos
        cursorPos=te.cursorPosition
        var replaceStringTemp=String.fromCharCode(8233)
        string=string.replace(/\n/g,replaceStringTemp)
        te.text=string
        te.cursorPosition=cursorPos
    }

    function displayColorText(){
        textProv=getText()
        textProv=" "+textProv
        colorWord(Backend.getFuncitionKeyWords(),"#45c6d6")
        colorWord(Backend.getVariableTypWords(),"#ff8080")
   /*     var search=new RegExp("//", "g")
        textProv=textProv.replace(search,"<font color=\"#a8abb0\">//")
*/
        var charIndex=0;
        var endIndex=0;

            while(charIndex>-1){
            charIndex =textProv.indexOf("//",charIndex);
            if(charIndex>=0){
                endIndex=textProv.indexOf("\n",charIndex+2)
                if(endIndex<0){
                    endIndex=textProv.length
                }

                textProv=textProv.substring(0,charIndex)+"<font color=\"#a8abb0\">"+deleteHtml(textProv.substring(charIndex,endIndex))+"</font>"+textProv.substring(endIndex)
                endIndex=textProv.indexOf("\n",charIndex)
                charIndex=endIndex
            }

        }
        textProv=textProv.substring(1);
        setText(textProv)
    }

    function colorWord(keyword,color){
        for(var i=0;i<keyword.length;i++){
            replaceString(keyword[i],"<font color=\""+color+"\">")
        }
    }

    function replaceString(word,fontStartText){
        var search=new RegExp("\n"+word+"\n", "g")
        textProv=textProv.replace(search,"\n"+fontStartText+word+"</font>\n")
        search=new RegExp("\n"+word+" ", "g")
        textProv=textProv.replace(search,"\n"+fontStartText+word+"</font> ")
        search=new RegExp(" "+word+"\n", "g")
        textProv=textProv.replace(search," "+fontStartText+word+"</font>\n")
        search=new RegExp(" "+word+" ", "g")
        textProv=textProv.replace(search," "+fontStartText+word+"</font> ")
    }
    function deleteHtml(stringProv){
        var charIndex=0
        var search=new RegExp("</font>", "g")
        stringProv=stringProv.replace(search,"")
        while(charIndex>-1){
            charIndex=stringProv.indexOf("<font",0)
            if(charIndex>-1){
                var charEnd
                charEnd=stringProv.indexOf(">",charIndex)
                if(charEnd>-1){
                    stringProv=stringProv.replace(stringProv.substring(charIndex,charEnd+1),"")
                }
            }
        }
        return stringProv
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
