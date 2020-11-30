#include "Programm.h"

Programm::Programm(QObject *parent) :
    QObject(parent){
    #ifdef RASPBERRYPI
    wiringPiSetup();
#endif
    DoubleJointMotor *dJ=new DoubleJointMotor(22,23,24,25,&tempo);
    doubleJointMotor.push_back(dJ);
    joints.push_back(new Joint(3680,-3680));
    joints[joints.size()-1]->setDoubleJointMotor(doubleJointMotor[0],0);
    joints.push_back(new Joint(4000,-4000));
    joints[joints.size()-1]->setDoubleJointMotor(doubleJointMotor[0],1);
    tempo=100;
    connect(this,SIGNAL(ProgrammFinished()),this,SLOT(nextProgramm()));
    for(unsigned long i=0; i<joints.size();i++){

        connect(joints[i],SIGNAL(ProgrammFinished()),this,SLOT(ProgrammFinishedSlot()));
    }
    for(unsigned long i=0; i<doubleJointMotor.size();i++){
        connect(doubleJointMotor[i],SIGNAL(ProgrammFinished()),this,SLOT(ProgrammFinishedSlot()));
    }
}

Programm::~Programm(){


}

void Programm::checkProgramm(QString _Programm){
    vector<QString> ProgrammVecTemp;
    ProgrammVecTemp=split(_Programm,'\n');
    programmVec.clear();

    for(int i=0;i<ProgrammVecTemp.size();i++){
        programmVec.push_back(Command(i,ProgrammVecTemp[i]));
    }
    if(errorList.size()<1){
        runningCommand=0;
        nextProgramm();
    }
    else{
       //printError();
    }
}


void Programm::turn(vector<QString> Programm){
    if(Programm.size()<3){
       //printError("Error: Programm for Turn to short: Example  turn J4 100",runningCommand);
    }
    else{
        Joint *j;

        if(Programm[1]=="J1"){
            j=joints[0];
        }
        else if(Programm[1]=="J2"){
            j=joints[1];
        }
        else{
           //printError("Error: Joint not found",runningCommand);
            return;
        }

        j->turnPosition(Programm[2].toInt());
    }
}

void Programm::setTempo(vector<QString> Programm){
    if(Programm.size()==2){
        tempo=Programm[1].toInt();
    }
    else{
       //printError("Error: Programm for Turn to short: Example tempo 100",runningCommand);
    }
    ProgrammFinishedSlot();
}

void  Programm::pause(vector<QString> Programm){
    if(Programm.size()==2){
        qDebug()<<"set Timer";
        QTimer::singleShot(Programm[1].toInt(), this, SLOT(ProgrammFinishedSlot()));
    }
    else{
       //printError("Error: Programm for Turn to short: Example pause 100",runningCommand);
    }
}
void Programm::stopJoints(){
    for(int i=0;i<doubleJointMotor.size();i++){
        doubleJointMotor[i]->running=false;
    }
}

void Programm::setPos(vector<QString> Programm){
    if(Programm.size()<3){
       //printError("Error: Programm for Turn to short: Example  set J4 100",runningCommand);
    }
    else{
        Joint *j;

        if(Programm[1]=="J1"){
            j=joints[0];
        }
        else if(Programm[1]=="J2"){
            j=joints[1];
        }
        else{
           //printError("Error: Joint not found",runningCommand);
            return;
        }

        j->setPosition(Programm[2].toInt());
    }
    ProgrammFinishedSlot();
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

void Programm::nextProgramm(){
    if(runningCommand<programmVec.size()){
        emit newRunningCommand(runningCommand);
        vector<QString> ProgrammVec;
        ProgrammVec=split(ProgrammVecTemp[runningCommand],' ');
        if(ProgrammVec.size()>0){
            if(ProgrammVec[0]==TEMPO_SYNTAX){
                setTempo(ProgrammVec);
            }
            else if(ProgrammVec[0]==TURN_SYNTAX_NAME){
                   turn(ProgrammVec);
               }
            else if(ProgrammVec[0]==PAUSE_SYNTAX){
                   pause(ProgrammVec);
               }
            else if(ProgrammVec[0]==SET_SYNTAX_NAME){
                   setPos(ProgrammVec);
               }
            else {
                errorList.newError(runningCommand,"Syntax unkown",ProgrammVecTemp[runningCommand])
            }
        }
        else{
            ProgrammFinishedSlot();
            return;
        }
    }
    else{
        if(loop){
            runningCommand=0;
            emit ProgrammFinished();
            return;
        }
    }
}

void Programm::ProgrammFinishedSlot(){
    runningCommand++;
    emit ProgrammFinished();
}
void Programm::printError(){
    emit errorOccured(errorList.getErrorLineVec(),errorList.getErrorMessageVec());
}
