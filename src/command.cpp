#include "command.h"


Bracket::Bracket (Command *arg_startOperation){
    startOperation=arg_startOperation;
}

bool Bracket::isClosed(){
    return closed;
}

void Bracket::close(Command *arg_endOperation){
    if(closed){
        throw(new Error("already closed"));
    }
    closed=true;
    endOperation=arg_endOperation;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



Command *Bracket::getEndOperation(){
    return endOperation;
}

Command *Bracket::getStartOperation(){
    return startOperation;
}
Command::Command(VariableSet *arg_varSet,BracketList *arg_brackets,vector<QString> arg_command,QObject *parent):
    QObject(parent)
{
    bcommand=BasicCommand::Empty;
    varSet=arg_varSet;
    brackets=arg_brackets;
    command=arg_command;
    if(command.size()>1){
        if(Variable::checkIfIsVariableType(command[0])){
            Operation::checkIfVarNameFree(varSet,command[1]);
            defineVar=new Variable(Variable::getVariableTypeFromString(command[0]),command[1]);
            arg_varSet->push_back(defineVar);
            command.erase(command.begin());
            command.shrink_to_fit();
            bcommand=BasicCommand::Define;
        }
    }
    if(command.size()>1){
        if(command[1]==OperatorSyntaxEqual){
            Operation::checkVarExist(varSet,command[0]);
            defineVar=Operation::getVariable(varSet,command[0]);
            vector<QString> tempVec(&command[2],&command[command.size()]);
            commandOperation.push_back(new Operation(varSet,tempVec));
            bcommand=BasicCommand::Set;
        }
        else if(checkIfIsBasicCommand(command[0])){
            setupBracktHead(command[0]);
        }
        else{
            throw(new Error("expression unkown 1"));
        }
    }
    else if(command.size()==1&&bcommand!=BasicCommand::Define){
        if(command[0]==Bracket_Closing){
            checkIfBracketsOpen();
            getOpeningBracket()->close(this);
            connect(getOpeningBracket()->getStartOperation(),SIGNAL(toEndBracket()),this,SLOT(toNextLine()));
            bcommand=BasicCommand::ClosingBracket;
        }
        else{

            throw(new Error("expression unkown 2"));
        }
    }

    /*programmLine=_programmLine;
    command=_command;
    globalVariables=&_globalVariables;
    checkCommand();*/

}
Command::~Command(){

}

void Command::checkIfAllBracketsClosed(){
    for(unsigned int i=0;i<brackets->size();i--){
        if(!((*brackets)[i]->isClosed())){
            QString closeing=Bracket_Closing;
            throw(new Error(closeing+" missing"));
        }
    }
}

BasicCommand Command::getBasicCommand(){
    return bcommand;
}

QVector<QString> Command::getBasicCommandName(){
  QVector<QString>  res;
  res.push_back(Basic_Command_While);
  res.push_back(Basic_Command_If);
  return res;
}

BasicCommand Command::getBasicCommandFromString(QString arg_name){
    //qDebug()<<arg_name
    if(Basic_Command_While==arg_name){
        return BasicCommand::While;
    }
    else if(Basic_Command_If==arg_name){
        return BasicCommand::If;
    }
    else{
        return BasicCommand::Empty;
    }
}

void Command::exec(){
    for(unsigned int i=0;i<commandOperation.size();i++){
        commandOperation[i]->exec();
    }
    switch(bcommand){
        case BasicCommand::Set:
            Operation::copyVariable(commandOperation[0]->getResult(),defineVar);
            toNextLine();
            break;
        case BasicCommand::If:
            if(commandOperation[0]->getResult()->getValuetoBool()){
                toNextLine();
            }
            else{
                emit toEndBracket();
            }
            break;
        case BasicCommand::While:
            if(commandOperation[0]->getResult()->getValuetoBool()){
                toNextLine();
            }
            else{
                emit toEndBracket();
            }
            break;
        case BasicCommand::ClosingBracket:
            switch(getOpeningBracket()->getStartOperation()->getBasicCommand()){
                case BasicCommand::If:
                    toNextLine();
                    break;
                case BasicCommand::While:
                    getOpeningBracket()->getStartOperation()->exec();
                    break;
            default:
                qDebug()<<"is not a valid Basiccommand";
                break;
            }

            break;
        default:
            toNextLine();
            break;
    }
}

void Command::toNextLine(){
    emit nextLine();
}

void Command::checkIfBracketsOpen(){
    for(unsigned int i=brackets->size();i>0;i--){
        if(!((*brackets)[i-1]->isClosed())){
            return;
        }
    }
    throw(new Error("extraneous closing brace ('}')"));
}

Bracket* Command::getOpeningBracket(){
    for(unsigned int i=brackets->size()-1;i>0;i--){
        if(!(*brackets)[i]->isClosed()||(*brackets)[i]->getEndOperation()==this){
            return (*brackets)[i];
        }
    }
    return (*brackets)[0];
}
bool Command::checkIfIsBasicCommand(QString arg_name){
    QVector<QString> vec= getBasicCommandName();
    for (int i=0;i<vec.size();i++) {
        if(vec[i]==arg_name){
            return true;
        }
    }
    return false;
}
void Command::checkString(QString &arg_currentString,QString arg_expectedString){
    if(arg_currentString!=arg_expectedString){
        throw(new Error("expected "+arg_expectedString));
    }
}

void Command::setupBracktHead(QString arg_name){
    if(command[0]==arg_name&&command.size()>3){
        checkString(command[1],OperationBeginSyntax);
        checkString(command[command.size()-2],OperationEndSyntax);
        checkString(command[command.size()-1],Bracket_Opening);
        vector<QString> tempVec(&command[2],&command[command.size()-2]);
        commandOperation.push_back(new Operation(varSet,tempVec));
        brackets->push_back(new Bracket(this));
        bcommand=getBasicCommandFromString(arg_name);
    }
    else{
        throw(new Error("expression expected"));
    }
}

