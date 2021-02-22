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
        varSet = new VariableSet;
        errorList = new ErrorList;
        ////---------------------------------------------------------------------------------////
        ////-----------------------------------PREPARATION-----------------------------------////
        ////---------------------------------------------------------------------------------////

        // save Program

        ofstream file("savefile.txt");
        file<< arg_program.toStdString();
        file.close();

        //make sure Operatorchar are clearly seperated
        vector<QString> findStr;
        findStr=SubOperation::getOperatorSyntax();
        for(unsigned int i=0;i<findStr.size();i++){
            arg_program.replace(findStr[i]," "+findStr[i]+" ");
        }
        QString prov;
        prov=OperatorSyntaxEqual;
        prov+="  ";
        prov+=OperatorSyntaxMinus;
        QString prov1;
        prov1=prov+" ";
        arg_program.replace(prov1,prov);

        prov=subOperationBeginSyntax;
        prov+="  ";
        prov+=OperatorSyntaxMinus;
        prov1=prov+" ";
        arg_program.replace(prov1,prov);
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

        for(unsigned int i=0;i<stringProgram.size();i++){
            try{
                for(unsigned int j=0;j<stringProgram[i].size();j++){
                    if(SubOperation::isNumber(stringProgram[i][j])){
                        Operation op(varSet,"#int_"+stringProgram[i][j],VariableType::Integer,stringProgram[i][j]);
                        operationArray.push_back(op);
                        unsigned int y;
                        y=j+1;
                        for(unsigned int x=i;x<stringProgram.size();x++){
                            while(y<stringProgram[x].size()){
                                if(stringProgram[i][j]==stringProgram[x][y]){
                                    stringProgram[x][y]="#int_"+stringProgram[i][j];
                                }
                                y++;

                            }
                            y=0;
                        }
                        stringProgram[i][j]="#int_"+stringProgram[i][j];

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
            unsigned int startPos=0;
            if(stringProgram[i].size()>1){
                //check for defines
                if(Variable::checkIfIsVariableType(stringProgram[i][0])){
                     startPos=1;
                     if(stringProgram[i][1].left(1)=="#"){
                         throw(new Error("# is reserved for program variables"));
                     }
                     operationArray.push_back(Operation(varSet,stringProgram[i][1],stringProgram[i][0]));
                }
                SubOperation::checkVarExist(varSet,stringProgram[i][startPos]);
                //check for Operations
                if(stringProgram[i].size()>startPos+1){
                    if(stringProgram[i][startPos+1]==OperatorSyntaxEqual){
                        vector<QString> operatorVec(&stringProgram[i][startPos+2],&stringProgram[i][stringProgram[i].size()]);
                        operationArray.push_back(Operation(varSet,operatorVec,SubOperation::getVariable(varSet,stringProgram[i][startPos])));
                    }
                }
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
        for(unsigned long i=0;i<operationArray.size();i++){
            operationArray[i].exec();
            }
        print("\n-------------------------End Program-------------------------\n");
        print("\n-----------------------System Variable-----------------------\n");
        for(unsigned i=0;i<varSet->size();i++){
            if((*varSet)[i]->getName().left(1)=="#")
                print((*varSet)[i]->getName()+" = "+QString::number((*varSet)[i]->getValuetoInt()));
        }
        print("\n------------------------User Variable------------------------\n");
        for(unsigned i=0;i<varSet->size();i++){
            if((*varSet)[i]->getName().left(1)!="#")
                print((*varSet)[i]->getName()+" = "+QString::number((*varSet)[i]->getValuetoInt()));
        }
    }
    else{
        print("\n------------------------Error Occured------------------------\n");
        emit errorOccured();
    }
    print("\n-------------------------End Program-------------------------\n");
    emit programmFinished();
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

void Programm::print(QString arg_print){
    emit newOutput(arg_print);
    //qDebug()<<arg_print;
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
