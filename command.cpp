#include "command.h"


Command::Command(GlobalVariables& _globalVariables,int _programmLine,
                 QString _command,QObject *parent):
    QObject(parent)
{
    programmLine=_programmLine;
    command=_command;
    globalVariables=&_globalVariables;
    checkCommand();

}
Command::~Command(){

}

void Command::checkCommand(){
    try{

        vector<QString> commandVec;
        commandVec=split(command,' ');
        commandVec.erase(remove(commandVec.begin(),commandVec.end(),""),commandVec.end());
        commandVec.shrink_to_fit();
        if(commandVec.size()>0){
            if(checkWordBeginnig(commandVec[0],DEF_COMMENT_SYNTAX)){
                function=Function::comment;
                return;
            }
            else if(commandVec[0]==TEMPO_SYNTAX){
                try{
                    function=Function::tempo;
                    checkLength(commandVec,2);
                    checkNumber(commandVec[1]);
                }
                catch(Error *er){
                    er->addToMessage("tempo");
                    throw er;
                }
            }
            else if(commandVec[0]==TURN_SYNTAX_NAME){
                try{
                    function=Function::turn;
                    checkLength(commandVec,3);
                    setJoint(commandVec[1]);
                    checkNumber(commandVec[2]);
                }
                catch(Error *er){
                    er->addToMessage("turn");
                    throw er;
                }
            }
            else if(commandVec[0]==PAUSE_SYNTAX){
                try{
                    function=Function::pause;
                    checkLength(commandVec,2);
                    checkNumber(commandVec[1]);
                }
                catch(Error *er){
                    er->addToMessage("pause");
                    throw er;
                }
               }
            else if(commandVec[0]==SET_SYNTAX_NAME){
                try{
                    function=Function::set;
                    checkLength(commandVec,3);
                    setJoint(commandVec[1]);
                    checkNumber(commandVec[2]);
                }
                catch(Error *er){
                    er->addToMessage("set");
                    throw er;
                }
            }
            else if(commandVec[0]==DEF_DOUBLEMOTOR_SYNTAX){
                try{
                    function=Function::set;
                    checkLength(commandVec,3);
                    setJoint(commandVec[1]);
                    checkNumber(commandVec[2]);
                }
                catch(Error *er){
                    er->addToMessage("doublemotor definition");
                    throw er;
                }
            }
            else if(commandVec[0]==LABLE_SYNTAX){
                try{

                    checkLength(commandVec,2);
                    Label l;
                    l.gotoLine=programmLine;
                    l.labelName=commandVec[1];
                    globalVariables->lableVec.push_back(l);
                    function=Function::label;
                }

                catch(Error *er){
                    er->addToMessage("label");
                    throw er;
                }
            }
            else if(commandVec[0]==GOTO_SYNTAX){
                try{
                    checkLength(commandVec,2);
                    function=Function::gotoRobo;

                }
                catch(Error *er){
                    er->addToMessage("goto");
                    throw er;
                }
            }
            else if(commandVec[0]==LOOPSTART_SYNTAX){
                try{

                    Loop l;
                    l.startLine=programmLine;
                    if(commandVec.size()==1){
                        l.endlessLoop=true;

                    }
                    else{
                        checkLength(commandVec,2);
                        checkNumber(commandVec[1]);
                        l.starLoopCount=commandVec[1].toInt();
                    }
                    function=Function::loopStart;
                    globalVariables->loopVec.push_back(l);
                }
                catch(Error *er){
                    er->addToMessage("loopStart");
                    throw er;
                }
            }
            else if(commandVec[0]==LOOPEND_SYNTAX){
                try{
                    checkLength(commandVec,1);
                    function=Function::loopEnd;
                    Loop *l;
                    bool found=false;
                    for(int i=globalVariables->loopVec.size();i>0;i--){
                        if(!globalVariables->loopVec[i-1].defined){
                           found=true;
                           globalVariables->loopVec[i-1].defined=true;
                           globalVariables->loopVec[i-1].endLine=programmLine;
                           break;
                        }
                    }
                    if(!found){
                        throw (new Error(programmLine,"Loop start not found",command));
                    }
                }
                catch(Error *er){
                    er->addToMessage("loopEnd");
                    throw er;
                }
            }
            else {
                throw (new Error(programmLine,"Syntax unkown",command));
            }
        }
        else{
            function=Function::empty;
            return;
        }
    }
    catch(Error *er){
        er->addToMessage("Compile ");
        throw er;
    }
}
void Command::exec(){
    vector<QString> commandVec;
    commandVec=split(command,' ');
    commandVec.erase(remove(commandVec.begin(),commandVec.end(),""),commandVec.end());
    commandVec.shrink_to_fit();
    if(function!=error){
        emit commandStart(programmLine);
    }
    try{
        switch (function) {
               case pause:
                    QTimer::singleShot(commandVec[1].toInt(), this, SLOT(commandFinishedSlot()));
                    break;
                case set:
                    joint->setPosition(commandVec[2].toInt());
                    commandFinishedSlot();
                    break;
                case turn:
                    connect(joint,SIGNAL(commandFinished()),this,SLOT(commandFinishedSlot()));
                    joint->turnPosition(commandVec[2].toInt());
                    break;
                case tempo:
                    globalVariables->tempo=commandVec[1].toInt();
                    commandFinishedSlot();
                    break;
                case comment:
                    commandFinishedSlot();
                    break;
                case empty:
                    commandFinishedSlot();
                    break;
                case label:
                    commandFinishedSlot();
                    break;
                case loopStart:
                    for(unsigned i=0;i<globalVariables->loopVec.size();i++){
                        if(globalVariables->loopVec[i].endlessLoop||globalVariables->loopVec[i].startLine==programmLine){
                            globalVariables->loopVec[i].loopCount++;
                            commandFinishedSlot();
                            break;
                        }
                    }
                    break;
                case loopEnd:
                    for(unsigned i=0;i<globalVariables->loopVec.size();i++){
                        if(globalVariables->loopVec[i].endLine==programmLine){
                            if(globalVariables->loopVec[i].endlessLoop||globalVariables->loopVec[i].loopCount<globalVariables->loopVec[i].starLoopCount){
                                emit gotoCommand(globalVariables->loopVec[i].startLine);
                                break;}
                            else{
                                globalVariables->loopVec[i].loopCount=0;
                                commandFinishedSlot();
                                break;
                            }
                        }
                    }
                    break;
                case gotoRobo:
                    unsigned int gotoline=0;
                    try{
                        getLable(commandVec[1],gotoline);
                        emit gotoCommand(gotoline);
                        break;
                    }
                    catch(Error *er){
                        er->addToMessage("goto");
                        throw er;
                        break;
                    }
                    break;





        }
    }
    catch(Error *er){
        er->addToMessage("Execute");
        emit executeError((*er));
    }
}
/*else{
    if(loop){
        runningCommand=0;
        emit ProgrammFinished();
        return;
    }
}*/


Function Command::getFunction(){
    return function;
}
unsigned int Command::getLine(){
    return programmLine;
}

vector<QString> Command::split(QString _str, char delimiter) {
  string str=_str.toStdString();
    vector<QString> internal;
  stringstream ss(str); // Turn the string into a stream.
  string tok;

  while(getline(ss, tok, delimiter)) {
    internal.push_back(QString::fromStdString(tok));
  }

  return internal;
}
void Command::getLable(QString name,unsigned int &_line){
    for(int i=0;i< globalVariables->lableVec.size();i++){
        if(globalVariables->lableVec[i].labelName==name){
            _line=globalVariables->lableVec[i].gotoLine;
            return;
        }
    }
    throw (new Error(programmLine,"Label unkown",command));
}
void Command::setJoint(QString name){
    for(unsigned long i=0;i<globalVariables->joints.size();i++){
        if(name==globalVariables->joints[i]->getName()){
            joint= globalVariables->joints[i];
            return;
        }
    }
    Error *er=new Error(programmLine,"Joint '"+ name+"' not defined",command);
    throw er;
}

void Command::checkNumber(QString number){
    bool ok;
    number.toInt(&ok,10);
    if(!ok){
        throw(new Error(programmLine,number+" isn't a number",command));
    }
}
bool Command::checkWordBeginnig(QString word,QString beginning){
        if(word.size()>=beginning.size()){
            for(int i=0;i<beginning.size();i++){
                if(word[i]!=beginning[i]){
                    return false;
                }
            }
            return true;
        }
        return false;
}

void Command::checkLength(vector<QString> &com,unsigned long len){
    if(com.size()>len){
        throw(new Error(programmLine,"to many arguments: found: "+QString::number(com.size())+" expected: "+QString::number(len),command));
    }
    if(com.size()<len){
        throw(new Error(programmLine,"to few arguments: found: "+QString::number(com.size())+" expected: "+QString::number(len),command));
    }
}

void Command::commandFinishedSlot(){
    switch (function) {
            case turn:
                disconnect(joint,SIGNAL(commandFinished()),this,SLOT(commandFinishedSlot()));
                break;
    }
    //qDebug()<<programmLine<<" Finished";

    emit commandFinished();
}
