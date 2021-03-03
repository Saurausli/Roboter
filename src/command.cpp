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
Command *Bracket::getEndOperation(){
    return endOperation;
}

Command *Bracket::getStartOperation(){
    return startOperation;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

IfElse::IfElse (Command *arg_ifOperation,Bracket *arg_parentBracket){
    ifOperation=arg_ifOperation;
    parentBracket=arg_parentBracket;
}

bool IfElse::isClosed(){
    return closed;
}

void IfElse::close(){
    /*if(closed){
        throw(new Error("If already closed"));
    }*/
    closed=true;
}

void IfElse::close(Command *arg_elseOperation){
    close();
    elseDefined=true;
    elseOperation=arg_elseOperation;
}

Command *IfElse::getElseOperation(){
    return elseOperation;
}

Command *IfElse::getIfOperation(){
    return ifOperation;
}

Bracket *IfElse::getBracket(){
    return parentBracket;
}

bool IfElse::isElseDefined(){
    return elseDefined;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





Command::Command(ProgramData *arg_programData,vector<QString> arg_command, QObject *parent):
    QObject(parent)
{
    bcommand=BasicCommand::Empty;

    programData=arg_programData;
    command=arg_command;
    parentBracket=getParentBracket();
    if(command.size()>1){
        if(Variable::checkIfIsVariableType(command[0])){
            Operation::checkIfVarNameFree(&programData->varSet,command[1]);
            defineVar=new Variable(Variable::getVariableTypeFromString(command[0]),command[1]);
            programData->varSet.push_back(defineVar);
            command.erase(command.begin());
            command.shrink_to_fit();
            bcommand=BasicCommand::Define;
        }
    }
    if(command.size()>1){
        if(command[1]==OperatorSyntaxEqual){
            Operation::checkVarExist(&programData->varSet,command[0]);
            defineVar=Operation::getVariable(&programData->varSet,command[0]);
            vector<QString> tempVec(&command[2],&command[command.size()]);
            commandOperation.push_back(new Operation(&programData->varSet,tempVec));
            bcommand=BasicCommand::Set;
        }
        /*else if(command[0]==Basic_Command_Else&&command[1]==Bracket_Opening&&command.size()==2){
            vector<QString> tempVec(&command[2],&command[command.size()-2]);

            programData->bracketList.push_back(new Bracket(this));
            if(programData->ifElseList.size()>0){
                if(programData->ifElseList[programData->ifElseList.size()-1]->isClosed()){
                    throw(new Error("expected expression"));
                }
                programData->ifElseList[programData->ifElseList.size()-1]->close(this);
                bcommand=BasicCommand::Else;
            }
            else{
                throw(new Error("expected expression"));
            }

        }*/
        else if(checkIfIsBasicCommand(command[0])){
            if(command[0]==Basic_Command_Else){
                    if(getOpenIfElse()==nullptr){
                        throw(new Error("if statment missing 1"));
                    }
                    if(parentBracket==getOpenIfElse()->getBracket()){

                        getOpenIfElse()->close(this);

                    }
                    else{
                       throw(new Error("if statment missing 2"));
                    }
            }
            setupBracktHead(command[0]);
            if(bcommand==BasicCommand::If){
                programData->ifElseList.push_back(new IfElse(this,parentBracket));
            }
        }
        else{
            throw(new Error("expression unkown 1"));
        }
    }

    else if(command.size()==1&&bcommand!=BasicCommand::Define){
        if(command[0]==Bracket_Closing){
            checkIfBracketsOpen();
            connect(parentBracket->getStartOperation(),SIGNAL(toEndBracket()),this,SLOT(toNextLine()));
            parentBracket->close(this);
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
    if(bcommand!=BasicCommand::Empty&&bcommand!=BasicCommand::If&&bcommand!=BasicCommand::Else&&programData->ifElseList.size()>0){
        IfElse *ifElse_ptr=getOpenIfElse();
        if(ifElse_ptr!=nullptr){
            if(parentBracket==ifElse_ptr->getBracket()){
                ifElse_ptr->close();
            }
        }
    }
}
Command::~Command(){

}

void Command::checkIfAllBracketsClosed(){
    for(unsigned int i=0;i<programData->bracketList.size();i--){
        if(!(programData->bracketList[i]->isClosed())){
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
    res.push_back(Basic_Command_Else);
    res.push_back(Basic_Command_Main);
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
    else if(Basic_Command_Else==arg_name){
        return BasicCommand::Else;
    }
    else if(Basic_Command_Main==arg_name){
        return BasicCommand::Main;
    }
    else{
        return BasicCommand::Empty;
    }
}
OperationList* Command::getOperationList(){
    return &commandOperation;
}

void Command::exec(){
    qDebug()<<"\n"<<command<<"\n";
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
        case BasicCommand::Else:
            qDebug()<<!(*getIfElseFromElse()->getIfOperation()->getOperationList())[0]->getResult()->getValuetoBool();
            if(!(*getIfElseFromElse()->getIfOperation()->getOperationList())[0]->getResult()->getValuetoBool()){
                toNextLine();
            }
            else{
                emit toEndBracket();
            }
            break;

        case BasicCommand::ClosingBracket:
            switch(getBracket()->getStartOperation()->getBasicCommand()){
                case BasicCommand::If:
                    toNextLine();
                    break;
                case BasicCommand::While:
                    getBracket()->getStartOperation()->exec();
                    break;
                case BasicCommand::Else:
                    toNextLine();
                    break;
                case BasicCommand::Main:
                    toNextLine();
                    break;
                default:
                    qDebug()<<"is not a valid Basiccommand";
                    break;
            }
            break;
        case BasicCommand::Main:
            toNextLine();
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
    for(unsigned int i=programData->bracketList.size();i>0;i--){
        if(!(programData->bracketList[i-1]->isClosed())){
            return;
        }
    }
    throw(new Error("extraneous closing brace ('}')"));
}

Bracket* Command::getBracket(){
    for(int i=int(programData->bracketList.size()-1);i>=0;i--){
        if(!(programData->bracketList[unsigned(i)]->isClosed())||programData->bracketList[unsigned(i)]->getEndOperation()==this){
            return programData->bracketList[unsigned(i)];
        }
    }
    return nullptr;
}

Bracket* Command::getParentBracket(){
    for(int i=int(programData->bracketList.size()-1);i>=0;i--){
        if(!(programData->bracketList[unsigned(i)]->isClosed())){
            return programData->bracketList[unsigned(i)];
        }
    }
    return nullptr;
}

IfElse* Command::getOpenIfElse(){
    for(int i=int(programData->ifElseList.size())-1;i>=0;i--){
        if(!(programData->ifElseList[unsigned(i)]->isClosed())){
            return programData->ifElseList[unsigned(i)];
        }
    }
    return nullptr;
}

IfElse* Command::getIfElseFromElse(){
    for(unsigned i=0;i<programData->ifElseList.size();i++){
        if(programData->ifElseList[unsigned(i)]->getElseOperation()==this){
            return programData->ifElseList[unsigned(i)];
        }
    }
    return nullptr;
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

int Command::getIntArgument(BasicCommand arg_bcommand){
    switch(arg_bcommand){
        case BasicCommand::If:
            return 1;
        case BasicCommand::While:
            return 1;
        case BasicCommand::Else:
            return 0;
        case BasicCommand::Main:
            return 0;
    default:
        return -1;
    }
}

void Command::checkString(QString &arg_currentString,QString arg_expectedString){
    if(arg_currentString!=arg_expectedString){
        throw(new Error("expected "+arg_expectedString));
    }
}

void Command::setupBracktHead(QString arg_name){
    if(command[0]==arg_name&&command.size()>1){

        checkString(command[command.size()-1],Bracket_Opening);

        bcommand=getBasicCommandFromString(arg_name);
        int argument=getIntArgument(bcommand);
        if(argument>0&&command.size()>3){
            checkString(command[1],OperationBeginSyntax);
            checkString(command[command.size()-2],OperationEndSyntax);
        }
        if(argument==0){
            if(command.size()>2){
                throw(new Error("expression not expected"));
            }
        }
        else{
            vector<QString> tempVec(&command[2],&command[command.size()-2]);
            commandOperation.push_back(new Operation(&programData->varSet,tempVec));
        }
        programData->bracketList.push_back(new Bracket(this));
    }
    else{
        throw(new Error("expression expected"));
    }
}

