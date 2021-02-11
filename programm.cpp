#include "programm.h"

Programm::Programm(QObject *parent) :
    QObject(parent){
    #ifdef RASPBERRYPI
    wiringPiSetup();
#endif

}

Programm::~Programm(){


}
void Programm::compileProgram(QString arg_program){
    try {

        ////---------------------------------------------------------------------------------////
        ////-----------------------------------PREPARATION-----------------------------------////
        ////---------------------------------------------------------------------------------////

        // save Program

        ofstream file("savefile.txt");
        file<< arg_program.toStdString();
        file.close();

        // stringProgram[line][word]

        vector<vector<QString>> stringProgram;
        vector<QString> lineVec;

        // Split Programm into lines

        lineVec=split(arg_program,'\n');
        for(unsigned long i=0; i<lineVec.size();i++){ // Split Programm into words
            stringProgram.push_back(split(lineVec[i],' '));
            stringProgram[i].erase(remove(stringProgram[i].begin(),stringProgram[i].end(),""),stringProgram[i].end());
            stringProgram[i].shrink_to_fit();
        }

        // replace numbers with Variables
        for(unsigned long i=0;i<stringProgram.size();i++){
            for(unsigned long j=0;j<stringProgram[i].size();j++){
                if(isNumber(stringProgram[i][j])){
                    compDefineVariable("#int_"+stringProgram[i][j],VariableType::integer,stringProgram[i][j].toInt(),false);
                    stringProgram[i][j]="#int_"+stringProgram[i][j];
                }
            }
        }
        varVec.clear();

        ////---------------------------------------------------------------------------------////
        ////------------------------------------Compiling------------------------------------////
        ////---------------------------------------------------------------------------------////


        for(unsigned long i=0;i<stringProgram.size();i++){
            if (stringProgram[i].size()>0) {
                if(stringProgram[i][0].mid(0,2)!="//"){

                    if(stringProgram[i].size()>1){
                        unsigned startPos=1;
                        if(stringProgram[i][0]==VariableSyntaxInteger){
                            compDefineVariable(stringProgram[i][1],VariableType::integer);
                            startPos=2;
                        }
                        if(stringProgram[i].size()>startPos+2){
                            if(stringProgram[i][startPos]==OperatorSyntaxEqual){
                                if(stringProgram[i].size()>stringProgram[i].size()>startPos+1){
                                   if(stringProgram[i].size()>stringProgram[i].size()==startPos+2){
                                       searchForVariable(stringProgram[i][startPos+1]);
                                       operationArray.push_back(new Operation(getVariable(stringProgram[i][startPos+1]),getVariable(stringProgram[i][startPos-1])));
                                   }
                                }
                                else{
                                    throw(new Error("expected expression"));
                                }
                            }
                        }
                    }
                }
            }
        }
        //varVec.clear();
        /*
        for(unsigned long i=0;i<stringProgram.size();i++){
            if(stringProgram[i].size()>1){
                if(VariableSyntaxInteger==stringProgram[i][0]){
                    varVec.push_back(new Variable(VariableType::integer,stringProgram[i][1]));
                    if(stringProgram[i].size()==4){
                        if(stringProgram[i][2]==OperatorSyntaxEqual){
                            bool ok;
                            stringProgram[i][3].toInt(&ok,10);
                            if(!ok){
                                if(getVariable(stringProgram[i][3],varVec)!=nullptr){
                                    varVec[varVec.size()-1]->setValue(stringProgram[i][3]);
                                }
                            }
                            else {
                                varVec[varVec.size()-1]->setValue(stringProgram[i][3]);
                            }

                        }
                    }
                }
                else if(stringProgram[i][1]==OperatorSyntaxEqual){
                    if(stringProgram[i].size()==5){
                        vector<Variable*> provOperVec;

                        provOperVec.push_back(getVariable(stringProgram[i][2],varVec));
                        provOperVec.push_back(getVariable(stringProgram[i][4],varVec));
                        Operation op(provOperVec,getVariable(stringProgram[i][0],varVec),Operation::getOperator(stringProgram[i][3]));
                        op.setResultVariable(getVariable(stringProgram[i][0],varVec));
                        operationArray.push_back(&op);
                        qDebug()<<op.getResult()->getValuetoInt();
                }
            }
        }
    }*/

    //execute Program

    for(unsigned long i=0;i<operationArray.size();i++){
        operationArray[i]->exec();
        }
    }
    for
    catch(Error *er){
        qDebug()<<er->getMessage();
    }
}

vector<QString> Programm::split(QString _str, char delimiter) {
  string str=_str.toStdString();
   vector<QString> internal;
  stringstream ss(str); // Turn the string into a stream.
  string tok;

  while(getline(ss, tok, delimiter)) {
    internal.push_back(QString::fromStdString(tok));
  }

  return internal;
}
bool Programm::isNumber(QString &arg_string){
    bool ok;
    arg_string.toInt(&ok,10);
    return ok;
}
void Programm::searchForVariable(QString arg_name){
    for(unsigned long i=0;i<varVec.size();i++){
        if(varVec[i]->getName()==arg_name){
            return;
        }
    }
    throw(new Error(arg_name+" not defined"));
}

void Programm::compDefineVariable(QString arg_name,VariableType arg_type){
    compDefineVariable(arg_name,arg_type,QVariant(0));
}

void Programm::compDefineVariable(QString arg_name,VariableType arg_type,QVariant arg_value, bool error){
    bool free=true;
    for(unsigned long i=0;i<varVec.size();i++){
        if(varVec[i]->getName()==arg_name){
            free= false;
        }
    }
    if(free){
        operationArray.push_back(new Operation(new Variable(arg_type,arg_name,arg_value),Operator::defineVariable,&varVec));
        varVec.push_back(operationArray[operationArray.size()-1]->getResult());
    }
    else{
        if(error){
            throw(new Error(arg_name+" already exist"));
        }
    }
}

Variable* Programm::getVariable(QString arg_name){
    return getVariable(arg_name,varVec);
}

Variable* Programm::getVariable(QString arg_name, vector<Variable*> &arg_varVec){
    for(unsigned long i=0;i<arg_varVec.size();i++){
        if(varVec[i]->getName()==arg_name){
            return arg_varVec[i];
        }
    }
    return nullptr;
}
/*
bool Programm::checkProgramm(QString _Programm){
    *//*
     Doublemotor M1M2 22 23 24 25
     Joint J1 M1M2 0 3680 -3680
     Joint J2 M1M2 1 4000 -4000
     *//*
    globalVaribles= new GlobalVariables;
    /*DoubleJointMotor *dJ=new DoubleJointMotor("hallo",22,23,24,25,&globalVaribles->tempo);
    globalVaribles->doubleJointMotor.push_back(dJ);
    globalVaribles->joints.push_back(new Joint("J1",3680,-3680));
    globalVaribles->joints[globalVaribles->joints.size()-1]->setDoubleJointMotor(globalVaribles->doubleJointMotor[0],0);
    globalVaribles->joints.push_back(new Joint("J2",4000,-4000));
    globalVaribles->joints[globalVaribles->joints.size()-1]->setDoubleJointMotor(globalVaribles->doubleJointMotor[0],1);*//*
    globalVaribles->tempo=100;

    //connect(this,SIGNAL(ProgrammFinished()),this,SLOT(nextProgramm()));
    for(unsigned long i=0; i<globalVaribles->joints.size();i++){

        //connect(globalVaribles->joints[i],SIGNAL(ProgrammFinished()),this,SLOT(ProgrammFinishedSlot()));
    }
    for(unsigned long i=0; i<globalVaribles->doubleJointMotor.size();i++){
        //connect(globalVaribles->doubleJointMotor[i],SIGNAL(ProgrammFinished()),this,SLOT(ProgrammFinishedSlot()));
    }
    ofstream file("savefile.txt");
    emit newRunningProgramm(_Programm);
    file<< _Programm.toStdString();
    file.close();
    vector<QString> ProgrammVecTemp;
    ProgrammVecTemp=Command::split(_Programm,'\n');
    programmVec.clear();
    errorList.clear();
    for(unsigned long i=0;i<ProgrammVecTemp.size();i++){
        try{
            programmVec.push_back(new Command((*globalVaribles),int(i),ProgrammVecTemp[i]));
        }
        catch(Error *er){
            qDebug()<<er->getMessage();
            errorList.push_back((*er));

        }
    }
    emit errorOccured();
    if(errorList.size()>0){
        return false;
    }
    else{
        runningCommand=0;
        return true;
    }
}

void Programm::connectProgramm(bool _loop){
    if(errorList.size()==0){
        for(unsigned long i=0;i<(programmVec.size()-1);i++){
            connect(programmVec[i],SIGNAL(commandFinished()),programmVec[i+1],SLOT(exec()));
        }
        if(_loop){
            connect(programmVec[programmVec.size()-1],SIGNAL(commandFinished()),programmVec[0],SLOT(exec()));
            loop=_loop;
        }
        else{
            connect(programmVec[programmVec.size()-1],SIGNAL(commandFinished()),this,SLOT(ProgrammFinishedSlot()));
        }
        for(unsigned long i=0;i<programmVec.size();i++){
            connect(programmVec[i],SIGNAL(commandStart(int)),this,SLOT(newRunningCommandSlot(int)));
            connect(programmVec[i],SIGNAL(gotoCommand(unsigned int)),this,SLOT(gotoSlot(unsigned int)));
            connect(programmVec[i],SIGNAL(executeError(Error)),this,SLOT(commandError(Error)));
        }
    }
}

void Programm::startProgramm(){
    programmVec[0]->exec();

}
void Programm::stopJoints(){
    for(unsigned long i=0;i<globalVaribles->doubleJointMotor.size();i++){
        globalVaribles->doubleJointMotor[i]->running=false;
    }
    for(unsigned long i=0;i<(programmVec.size()-1);i++){
        disconnect(programmVec[i],SIGNAL(commandFinished()),programmVec[i+1],SLOT(exec()));
    }
    disconnect(programmVec[programmVec.size()-1],SIGNAL(commandFinished()),programmVec[0],SLOT(exec()));
}


void Programm::newRunningCommandSlot(int lineID){
    runningCommand=lineID;
    emit newRunningCommand(runningCommand);
}

void Programm::ProgrammFinishedSlot(){
    emit programmFinished();
}

void Programm::gotoSlot(unsigned int line){
    runningCommand=line;
    for(unsigned long i=0;i<programmVec.size();i++){
        if(programmVec[i]->getLine()==line){
            programmVec[i]->exec();
            return;
        }
    }

}

void Programm::commandError(Error er){
    errorList.push_back(er);
    emit errorOccured();
    emit stopJoints();

}*/
