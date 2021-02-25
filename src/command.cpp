#include "command.h"


Command::Command(VariableSet *arg_varSet,vector<QString> arg_command,QObject *parent):
    QObject(parent)
{
    bcommand=BasicCommand::error;
    varSet=arg_varSet;
    if(arg_command.size()>1){
        if(Variable::checkIfIsVariableType(arg_command[0])){
            Operation::checkIfVarNameFree(varSet,arg_command[1]);
            defineVar=new Variable(Variable::getVariableTypeFromString(arg_command[0]),arg_command[1]);
            arg_varSet->push_back(defineVar);
            arg_command.erase(arg_command.begin());
            arg_command.shrink_to_fit();
            bcommand=BasicCommand::define;
        }
    }
    if(arg_command.size()==1&&bcommand!=BasicCommand::define){
        throw(new Error("expected expression"));
    }
    else if(arg_command.size()>1){
        if(arg_command[1]==OperatorSyntaxEqual){
            Operation::checkVarExist(varSet,arg_command[0]);
            defineVar=Operation::getVariable(varSet,arg_command[0]);
            vector<QString> tempVec(&arg_command[2],&arg_command[arg_command.size()]);
            commandOperation.push_back(new Operation(varSet,tempVec));
            bcommand=BasicCommand::set;
        }
    }

    /*programmLine=_programmLine;
    command=_command;
    globalVariables=&_globalVariables;
    checkCommand();*/

}
Command::~Command(){

}

void Command::exec(){
    for(unsigned int i=0;i<commandOperation.size();i++){
        commandOperation[i]->exec();
    }
    switch(bcommand){
        case BasicCommand::set:{
            Operation::copyVariable(commandOperation[0]->getResult(),defineVar);
        }
    }
}
/*
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
                    function=Function::define;
                    checkLength(commandVec,6);
                    checkNumber(commandVec[2]);
                    checkNumber(commandVec[3]);
                    checkNumber(commandVec[4]);
                    checkNumber(commandVec[5]);
                    globalVariables->doubleJointMotor.push_back(new DoubleJointMotor(commandVec[1],commandVec[2].toInt(),commandVec[3].toInt()
                                                                ,commandVec[4].toInt(),commandVec[5].toInt(),&globalVariables->tempo));
                }
                catch(Error *er){
                    er->addToMessage("doublemotor definition");
                    throw er;
                }
            }
            else if(commandVec[0]==DEF_JOINT_SYNTAX){
                try{
                    function=Function::define;
                    bool doubleMotor;
                    int pinBegin=0;
                    checkMinLength(commandVec,6);
                    checkMotor(commandVec[2],doubleMotor);
                    if(doubleMotor){
                       checkLength(commandVec,6);
                       pinBegin=1;
                    }
                    else{
                        checkLength(commandVec,5);
                    }
                    checkNumber(commandVec[3+pinBegin]);
                    checkNumber(commandVec[4+pinBegin]);
                    globalVariables->joints.push_back(new Joint(commandVec[1],commandVec[3+pinBegin].toInt(),commandVec[4+pinBegin].toInt()));
                    if(doubleMotor){
                        checkNumber(commandVec[3]);
                        checkValueMinMax(commandVec[3].toInt(),0,1);
                        globalVariables->joints[globalVariables->joints.size()-1]->setDoubleJointMotor(globalVariables->doubleJointMotor[findDoubleMotor(commandVec[2])],
                                commandVec[3].toInt());

                    }
                }
                catch(Error *er){
                    er->addToMessage("joint definition");
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
               case pause:{
                    QTimer::singleShot(commandVec[1].toInt(), this, SLOT(commandFinishedSlot()));
                    break;}
                case set:{
                    joint->setPosition(commandVec[2].toInt());
                    commandFinishedSlot();
            break;}
                case turn:{
                    connect(joint,SIGNAL(commandFinished()),this,SLOT(commandFinishedSlot()));
                    joint->turnPosition(commandVec[2].toInt());
                    break;}
                case tempo:{
                    globalVariables->tempo=commandVec[1].toInt();
                    commandFinishedSlot();
                    break;}
                case loopStart:{
                    for(unsigned i=0;i<globalVariables->loopVec.size();i++){
                        if(globalVariables->loopVec[i].endlessLoop||globalVariables->loopVec[i].startLine==programmLine){
                            globalVariables->loopVec[i].loopCount++;
                            commandFinishedSlot();
                            break;
                        }
                    }
                    break;}
                case loopEnd:{
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
                    break;}
                case gotoRobo:{
                    unsigned int gotoline=0;
                    try{
                        getLable(commandVec[1],gotoline);
                        emit gotoCommand(gotoline);
                        break;}
                    catch(Error *er){
                        er->addToMessage("goto");
                        throw er;
                    }
                }
                default:{
                    commandFinishedSlot();
                    break;
                }
        }
    }
    catch(Error *er){
        er->addToMessage("Execute");
        emit executeError((*er));
    }
}
else{
    if(loop){
        runningCommand=0;
        emit ProgrammFinished();
        return;
    }
}


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
*/
