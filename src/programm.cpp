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
        print("\n----------------------Start Preparation----------------------\n");
        comList.clear();
        comList.shrink_to_fit();
        varSet = new VariableSet;
        errorList = new ErrorList;
        brackets = new BracketList;
        ////---------------------------------------------------------------------------------////
        ////-----------------------------------PREPARATION-----------------------------------////
        ////---------------------------------------------------------------------------------////

        // save Program

        ofstream file("savefile.txt");
        file<< arg_program.toStdString();
        file.close();

        //make sure Operatorchar are clearly seperated and dont seperat between some char. For Exampel "=" and "==" and no "=" and " =  = "
        arg_program.replace("\t","");
        vector<QString> findStr;
        findStr=Operation::getOperatorSyntax();
        vector<QString> actualStr;
        actualStr=Operation::getOperatorSyntax();

        for(unsigned int i=0;i<findStr.size();i++){
            for(unsigned int j=i+1;j<actualStr.size();j++){
                actualStr[j].replace(findStr[i]," "+findStr[i]+" ");
            }
        }
        for(unsigned int i=0;i<actualStr.size();i++){
            arg_program.replace(actualStr[i]," "+findStr[i]+" ");
        }

        // stringProgram[line][word]

        vector<vector<QString>> stringProgram;
        vector<QString> lineVec;

        // Split Programm into lines

        lineVec=split(arg_program,'\n');

        // Split Programm into words
        for(unsigned long i=0; i<lineVec.size();i++){
            stringProgram.push_back(split(lineVec[i],' '));
            stringProgram[i].erase(remove(stringProgram[i].begin(),stringProgram[i].end(),""),stringProgram[i].end());
            stringProgram[i].shrink_to_fit();
        }
        // replace numbers with Variables
        for(unsigned int i=0;i<stringProgram.size();i++){
            try{
                for(unsigned int j=0;j<stringProgram[i].size();j++){
                    if(Operation::isNumber(stringProgram[i][j])){
                        varSet->push_back(new Variable(VariableType::Integer,"#int_"+stringProgram[i][j],stringProgram[i][j]));
                        replaceString(&stringProgram,stringProgram[i][j],"#int_"+stringProgram[i][j],i);
                        }
                    else if(Operation::isBool(stringProgram[i][j])){
                        varSet->push_back(new Variable(VariableType::Boolean,"#boolean_"+stringProgram[i][j],stringProgram[i][j]));
                        replaceString(&stringProgram,stringProgram[i][j],"#boolean_"+stringProgram[i][j],i);
                        }
                    }



            }
            catch(Error *er){
                er->setLine(i);
                errorList->push_back(er);
                print(er->getMessage());
            }
        }

        ////---------------------------------------------------------------------------------////
        ////------------------------------------Compiling------------------------------------////
        ////---------------------------------------------------------------------------------////
        print("\n-----------------------Start Compiling-----------------------\n");

        for(unsigned long i=0;i<stringProgram.size();i++){
            try{
                comList.push_back(new Command(varSet,brackets,stringProgram[i]));
                if(comList.size()>1&&errorList->size()==0){
                    connect(comList[i-1],SIGNAL(nextLine()),comList[i],SLOT(exec()));
                }
                if(i==stringProgram.size()-1&&errorList->size()==0){
                    comList[i]->checkIfAllBracketsClosed();
                    connect(comList[i],SIGNAL(nextLine()),this,SLOT(programFinished()));
                }
            }
            catch(Error *er){
                er->setLine(i);
                errorList->push_back(er);
                print(er->getMessage());
                }
            }
        }
    catch(Error *er){
        errorList->push_back(er);
    }
    //execute Program

    if(errorList->size()==0){
        print("\n------------------------Start Program------------------------\n");
        //for(unsigned long i=0;i<comList.size();i++){
        if(comList.size()>0){
            comList[0]->exec();
        }
        else{
            programFinished();
        }
    }
    else{
        print("\n------------------------Error Occured------------------------\n");
        emit errorOccured();
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

void Programm::programFinished(){
    print("\n-------------------------End Program-------------------------\n");
    print("\n-----------------------System Variable-----------------------\n");
    for(unsigned i=0;i<varSet->size();i++){
        if((*varSet)[i]->getName().left(1)=="#")
            print(Variable::getVariableTypeName((*varSet)[i]->getVariableType())+" "+(*varSet)[i]->getName()+" = "+(*varSet)[i]->getValue());
    }
    print("\n------------------------User Variable------------------------\n");
    for(unsigned i=0;i<varSet->size();i++){
        if((*varSet)[i]->getName().left(1)!="#")
            print(Variable::getVariableTypeName((*varSet)[i]->getVariableType())+" "+(*varSet)[i]->getName()+" = "+(*varSet)[i]->getValue());
    }
    emit programmFinished();
}

void Programm::print(QString arg_print){
    emit newOutput(arg_print);
    qDebug()<<arg_print;
}

void Programm::replaceString(vector<vector<QString>> *arg_vec,QString arg_currentString,QString arg_newString,  unsigned int startX){
    for(unsigned int x=startX;x<arg_vec->size();x++){
        for(unsigned int y=0;y<(*arg_vec)[x].size();y++){
            if((*arg_vec)[x][y]==arg_currentString){
                (*arg_vec)[x][y]=arg_newString;
            }
        }
    }
}

/*
bool Programm::checkProgramm(QString _Programm){
    *//*
     Doublemotor M1M2 22 23 24 25
     Joint J1 M1M2 0 3680 -3680
     Joint J2 M1M2 1 4000 -4000
     *//*
    globalVaribles= new GlobalVariables;
    *//*DoubleJointMotor *dJ=new DoubleJointMotor("hallo",22,23,24,25,&globalVaribles->tempo);
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
