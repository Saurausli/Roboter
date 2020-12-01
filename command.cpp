#include "command.h"


Command::Command(GlobalVariables* _globalVariables,int _programmLine,
                 QString _command,QObject *parent):
    QObject(parent)
{
    programmLine=_programmLine;
    command=_command;
    globalVariables=_globalVariables;
    checkCommand();

}
Command::~Command(){

}

void Command::checkCommand(){
    try{

        vector<QString> commandVec;
        commandVec=split(command,' ');
        if(commandVec.size()>0){
            if(commandVec[0]==TEMPO_SYNTAX){
                try{
                    function=Function::tempo;
                    checkLength(&commandVec,2);
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
                    checkLength(&commandVec,3);
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
                    checkLength(&commandVec,2);
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
                    checkLength(&commandVec,3);
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
                    checkLength(&commandVec,3);
                    setJoint(commandVec[1]);
                    checkNumber(commandVec[2]);
                }
                catch(Error *er){
                    er->addToMessage("set");
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
        er->addToMessage("Compile");
        throw er;
    }
}
void Command::exec(){
    vector<QString> commandVec;
    commandVec=split(command,' ');
    qDebug()<<"Command "<<programmLine;
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
                break;
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

void Command::checkLength(vector<QString> *com,unsigned long len){
    if(com->size()>len){
        throw(new Error(programmLine,"to many arguments",command));
    }
    if(com->size()<len){
        throw(new Error(programmLine,"to few arguments",command));
    }
}

void Command::commandFinishedSlot(){
    switch (function) {
            case turn:
                disconnect(joint,SIGNAL(commandFinished()),this,SLOT(commandFinishedSlot()));
                break;
    }
    qDebug()<<"Finished";
    emit commandFinished();
}
