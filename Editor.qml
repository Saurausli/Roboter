import QtQuick 2.10
import QtQuick.Controls 2.10
Rectangle{
    id:background
    color: "#2e2f30"
    property string text: ""
    property string textProv
    property bool lockBar: false
    property int liveCommand: -1
    property variant error: []
    property variant errorMessage: []
    property alias editor: textEditInput
    FontMetrics {
        id: fontMetrics
        font: textEditInput.font
    }
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
    Flickable{
        id:sideNum
        contentHeight: sideNumRec.height
        contentWidth: sideNumRec.width
        contentY: textflick.contentY
        width: sideNumRec.width
        anchors.top: background.top
        anchors.bottom: background.bottom
        anchors.left: parent.left
        enabled: false
        clip: true
        Rectangle{
            id:sideNumRec
            width:(0.8125*num.font.pointSize*(' '+(textEditInput.lineCount)).length)+10+num.anchors.leftMargin
            height: textEditInput.height+(textflick.anchors.margins*2)
            clip: true
            color: "#404244"
            Text {
                id:num
                textFormat:TextEdit.RichText
                renderType: Text.NativeRendering
                text:" 1 "
                Component.onCompleted: {
                    num.updateLineCount()
                }

                Connections{
                    target:textEditInput
                    onLineCountChanged: {
                        num.updateLineCount()
                    }
                    onCursorLineChanged:{
                        num.updateColorNum()
                    }
                }


                font: textEditInput.font
                anchors.top: parent.top
                anchors.topMargin: textflick.anchors.margins

                anchors.left: parent.left
                anchors.leftMargin: 20
                anchors.bottom: parent.bottom

                color: "#bec0c2"
                property int prevLinCount: 1
                function updateLineCount(){
                    removeColor()
                    if(prevLinCount>textEditInput.lineCount){
                        for(var j=prevLinCount;j>textEditInput.lineCount;j--){
                           text= text.replace(" "+j-1+" <br> "+j+" "," "+j-1+" ")
                        }
                    }
                    else if(prevLinCount<textEditInput.lineCount){
                        for(j=prevLinCount;j<textEditInput.lineCount;j++){
                           text= text.replace(" "+j+" "," "+j+" <br> "+(j+1)+" ")
                        }
                    }
                    prevLinCount=textEditInput.lineCount
                    updateColorNum()
                    /*textFormat=TextEdit.PlainText
                    text="<pre> 1 "
                    for(var i=2;i<=textEditInput.lineCount;i++){
                            text=text+"<br> "+i+" "
                        }
                    text=text+"</pre>"
                    updateColorNum()
                    textFormat=TextEdit.RichText*/
                }
                function updateColorNum(){
                    removeColor()
                    var search=new RegExp(" "+textEditInput.cursorLine+" ", "g")
                    text=text.replace(search," <b><font color=\"#d6c540\">"+textEditInput.cursorLine+"</font> ")
                    /*console.debug("-------------------------------------------------------")
                    console.debug(text)
                    console.debug("-------------------------------------------------------")*/
                }
                function removeColor(){
                    var search=new RegExp("<b><font color=\"#d6c540\">", "g")
                    text=text.replace(search,"")
                    search=new RegExp("</font>", "g")
                    text=text.replace(search,"")
                }
            }
        }
    }

    Flickable{
        id:textflick
        contentHeight: textEditInput.height
        contentWidth: textEditInput.width+10
        flickableDirection: Flickable.AutoFlickIfNeeded
        clip: true
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: sideNum.right
        anchors.margins: 10
        boundsMovement: Flickable.StopAtBounds
        ScrollBar.vertical: ScrollBar {
                parent: textflick.parent
                anchors.top: textflick.top
                anchors.left: textflick.right
                anchors.bottom: textflick.bottom
                policy: ScrollBar.AlwaysOn
            }
        ScrollBar.horizontal: ScrollBar {
                parent: textflick.parent
                anchors.top: textflick.bottom
                anchors.left: textflick.left
                anchors.right: textflick.right
                orientation: Qt.Horizontal
                policy: ScrollBar.AlwaysOn
                visible: textflick.width<textEditInput.width
            }

        TextEdit{
            id:textEditInput
            clip:true
            //renderType: Text.NativeRendering
            property int lengthPrevious: 0
            property int cursorLine: (cursorRectangle.y+fontMetrics.height)/fontMetrics.height
            onCursorLineChanged: {
                ensureVisible(cursorRectangle)
            }
            //renderType: Text.NativeRendering
            width: contentWidth<textflick.width ? textflick.width : contentWidth
            height: contentHeight<textflick.height ? textflick.height  : lineCount*fontMetrics.height
            selectByKeyboard: true
            selectByMouse: true
            font.pointSize : 16
            font.family: "Courier New"
            color:"#d6cf9a"
            selectedTextColor: "#bec0c2"
            selectionColor: "#1d545c"
            onCursorRectangleChanged: ensureVisible(cursorRectangle)
            textFormat:Text.RichText
            wrapMode: TextEdit.NoWrap
            text: ""
            property int prevLinCount: 0
            onLengthChanged: {
                if(!lockBar&&length!=lengthPrevious){
                    lengthPrevious=length;
                    if(lineCount-prevLinCount>1){
                        for(var i=cursorLine-1;i>lineCount-prevLinCount;i--){

                            displayColorLine(i);
                        }
                    }
                    prevLinCount=lineCount
                    displayColorLine(cursorLine);
                }
            }
            Menu {
                    id: contextMenu
                    //background:
                    MenuItem {
                        text: qsTr("Copy")
                        enabled: textEditInput.selectedText
                        onTriggered: textEditInput.copy()
                    }
                    MenuItem {
                        text: qsTr("Cut")
                        enabled: textEditInput.selectedText
                        onTriggered: textEditInput.cut()
                    }
                    MenuItem {
                        text: qsTr("Paste")
                        enabled: textEditInput.canPaste
                        onTriggered: textEditInput.paste()
                    }
                    MenuItem {
                        text: qsTr("Redo")
                        enabled: textEditInput.canPaste
                        onTriggered: textEditInput.redo()
                    }
                    MenuItem {
                        text: qsTr("Undo")
                        enabled: textEditInput.canPaste
                        onTriggered: textEditInput.undo()
                    }
            }
            Shortcut {
                sequence: StandardKey.Redo
                onActivated: {
                    console.debug(textEditInput.canUndo)
                    textEditInput.redo()
                }
            }
            Shortcut {
                sequence: StandardKey.Cut
                onActivated: {
                    console.debug(textEditInput.canUndo)
                    textEditInput.undo()
                }
            }
            Shortcut {
                sequence: StandardKey.Copy
                onActivated: textEditInput.copy()
            }
            Shortcut {
                sequence: StandardKey.Cut
                onActivated: {if(!textEditInput.readOnly) textEditInput.cut()}
            }
            Shortcut {
                sequence: StandardKey.Paste
                onActivated: {if(!textEditInput.readOnly) textEditInput.paste()}
            }
        }
    }
    function getProgramm(){
        return getText(0,textEditInput.length)
    }

    function getText(start,end){
        //console.debug("get text")
        var temp

        temp=textEditInput.getText(start,end)
        var replaceStringTemp=String.fromCharCode(8233)
        var search=new RegExp(replaceStringTemp, "g")
        temp=temp.replace(search,"\n")
        replaceStringTemp=String.fromCharCode(8232)
        search=new RegExp(replaceStringTemp, "g")
        temp=temp.replace(search,"\n")
        return temp
    }

    function setText(string){
        lockBar=true
        var cursorPos=textEditInput.cursorPosition

        string=toRichText(string)
        //textEditInput.text="<pre>"+string+"</pre>"

        lockBar=false
        displayColorTextAll()
        textEditInput.cursorPosition=cursorPos
        ensureVisible(textEditInput.cursorRectangle)

    }

    function toRichText(string){
        var replaceStringTemp="<br>"//String.fromCharCode(8233)
        string=string.replace(/\n/g,replaceStringTemp)//Replace plain \n with html "\n"
        return string
    }

    function displayColorTextAll(){
        for(var i=1;i<=textEditInput.lineCount;i++){
            displayColorLine(i)
        }
    }

    function displayColorLine(line){
        var start;
        var end;
        start=textEditInput.positionAt(0,line*fontMetrics.height-(fontMetrics.height/2))
        end=textEditInput.positionAt(textEditInput.width,line*fontMetrics.height-(fontMetrics.height/2))
        textProv=textEditInput.getText(start,end)
        displayColorText()
        /*if(line==1){
            textProv="<pre>"+textProv
        }*/
        if(line==textEditInput.lineCount){
           // textProv=textProv+"</pre>"
        }
        lockBar=true
        var cursorPos
        cursorPos=textEditInput.cursorPosition
        textProv=toRichText(textProv)
        textEditInput.remove(start,end)
        textEditInput.insert(start,textProv)
        textProv=textEditInput.getFormattedText(0,textEditInput.length)
        textProv=textProv.replace("white-space: pre-wrap","white-space: pre")
        //console.debug(textProv)
        textEditInput.text=textProv
        textEditInput.cursorPosition=cursorPos
        lockBar=false
        //console.debug(textEditInput.text)
    }

    function displayColorText(){
        textProv=" "+textProv+" "
        colorWord(Backend.getFuncitionKeyWords(),"#45c6d6")
        //colorWord(Backend.getVariableTypWords(),"#ff8080")
/*      var charIndex=0;
        var endIndex=0;

        while(charIndex>-1){ // Comments Coloring
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

        }*/
        textProv=textProv.substring(1);
        textProv=textProv.substring(0,textProv.length-1);
    }

    function colorWord(keyword,color){
        /*for(var i=0;i<keyword.length;i++){
            replaceString(keyword[i],"<font color=\""+color+"\">")
        }*/
        replaceString("pause","<font color=\""+color+"\">")
    }

    function replaceString(word,fontStartText){
        console.debug(word, textProv)
        var search=new RegExp(" "+word+" ", "g")
        textProv=textProv.replace(search," "+fontStartText+word+"</font> ")
        console.debug(word, textProv)
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

    function ensureVisible(re){
        if(!lockBar){
            var visibleLines
            visibleLines=Math.floor(textflick.height/fontMetrics.height)

            var firstVisibleLine
            firstVisibleLine =Math.floor((textflick.contentY/fontMetrics.height)+1)
            if(textEditInput.cursorLine>=firstVisibleLine+visibleLines){
                textflick.contentY=textEditInput.cursorLine*fontMetrics.height
            }

            else if(textEditInput.cursorLine<firstVisibleLine){
                textflick.contentY=(textEditInput.cursorLine-1)*fontMetrics.height
            }

            if(re.x>textflick.width+textflick.contentX){
                textflick.contentX=(re.x-textflick.width)+10
            }
            else if(re.x<textflick.contentX){
                textflick.contentX=re.x
            }
        }
     }
}


