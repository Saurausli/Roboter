import QtQuick 2.10
import QtQuick.Controls 2.10
Rectangle{
    id:background
    color: "#2e2f30"
    property string text: ""
    property string textProv
    FontMetrics {
        id: fontMetrics
        font: textEditInput.font
    }
    Flickable{
        id:textflick
        contentHeight: textEditInput.height
        contentWidth: textEditInput.width
        flickableDirection: Flickable.AutoFlickIfNeeded
        clip: true
        anchors.fill:parent
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
                anchors.bottom: textflick.bottom
                anchors.left: textflick.left
                anchors.right: textflick.right
                orientation: Qt.Horizontal
                policy: ScrollBar.AlwaysOn
                visible: textflick.width<textEditInput.width
            }
        TextEdit{
            id:textEditInput
            clip:true
            property int lengthPrevious: 0
            width: contentWidth<textflick.width ? textflick.width : contentWidth
            onWidthChanged: {
                console.debug(width)
            }
            height: contentHeight<textflick.height ? textflick.height  : lineCount*fontMetrics.height
            selectByKeyboard: true
            selectByMouse: true
            font.pointSize : 16
            font.family: "Courier New"
            color:"#d6cf9a"
            selectedTextColor: "#bec0c2"
            selectionColor: "#1d545c"
            onCursorRectangleChanged: ensureVisible(cursorRectangle)
            textFormat:TextEdit.RichText
            //onCursorRectangleChanged: editor.ensureVisible(cursorRectangle)
            onLengthChanged: {
                if(length!=lengthPrevious){
                    lengthPrevious=length;
                    displayColorText();
                }
            }
        }
    }
    function getText(){
        //console.debug("get text")
        var temp
        temp=textEditInput.getText(0,300)
        var replaceStringTemp=String.fromCharCode(8233)
        var search=new RegExp(replaceStringTemp, "g")
        temp=temp.replace(search,"\n")
        replaceStringTemp=String.fromCharCode(8232)
        search=new RegExp(replaceStringTemp, "g")
        temp=temp.replace(search,"\n")
        return temp
    }
    function setText(string){
        textEditInput.lockBar=true
        string="<pre>"+string+"</pre>"
        var cursorPos
        cursorPos=textEditInput.cursorPosition
        var replaceStringTemp=String.fromCharCode(8233)
        string=string.replace(/\n/g,replaceStringTemp)
        textEditInput.text=string
        //console.debug(textEditInput.text)
        textEditInput.cursorPosition=cursorPos
        textEditInput.lockBar=false
        //ensureVisible(textEditInput.cursorRectangle)
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
    function ensureVisible(re){
       /* if(!te.lockBar){
            editor.firstVisibleLine=Math.floor(((vScrollBar.position*te.contentHeight)/fontMetrics.height)+1)

            var cursorLine=((re.y+fontMetrics.height)/fontMetrics.height)
            if(cursorLine>=editor.firstVisibleLine+editor.visibleLines){
                scrollTo(Qt.Vertical,(cursorLine-(editor.visibleLines/2))/te.lineCount)
            }

            else if(cursorLine<editor.firstVisibleLine){
                scrollTo(Qt.Vertical,(cursorLine+(editor.visibleLines/2)-1)/te.lineCount)
            }
            //console.debug(re.x,">",te.contentWidth,"*",editor.hScrollBar.position,"+",(editor.width/2))
            if(re.x>te.contentWidth*editor.hScrollBar.position+(editor.width/2)){
                //scrollTo(Qt.Horizontal,((re.x+10)-(editor.width/2))/te.contentWidth)
            }

            /*else if(re.x<(editor.hScrollBar.position*te.contentWidth)-(editor.width/2)){

            }*//*
            num.updateSide()
        }*/
    }
    }


