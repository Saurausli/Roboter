#include "Programm.h"

Programm::Programm(QObject *parent) :
    QObject(parent){
    #ifdef RASPBERRYPI
    wiringPiSetup();
#endif
    DoubleJointMotor *dJ=new DoubleJointMotor(22,23,24,25,&globalVaribles.tempo);
    globalVaribles.doubleJointMotor.push_back(dJ);
    globalVaribles.joints.push_back(new Joint("J1",3680,-3680));
    globalVaribles.joints[globalVaribles.joints.size()-1]->setDoubleJointMotor(globalVaribles.doubleJointMotor[0],0);
    globalVaribles.joints.push_back(new Joint("J2",4000,-4000));
    globalVaribles.joints[globalVaribles.joints.size()-1]->setDoubleJointMotor(globalVaribles.doubleJointMotor[0],1);
    globalVaribles.tempo=100;
    //connect(this,SIGNAL(ProgrammFinished()),this,SLOT(nextProgramm()));
    for(unsigned long i=0; i<globalVaribles.joints.size();i++){

        //connect(globalVaribles.joints[i],SIGNAL(ProgrammFinished()),this,SLOT(ProgrammFinishedSlot()));
    }
    for(unsigned long i=0; i<globalVaribles.doubleJointMotor.size();i++){
        //connect(globalVaribles.doubleJointMotor[i],SIGNAL(ProgrammFinished()),this,SLOT(ProgrammFinishedSlot()));
    }
}

Programm::~Programm(){


}

void Programm::checkProgramm(QString _Programm){
    vector<QString> ProgrammVecTemp;
    ProgrammVecTemp=Command::split(_Programm,'\n');
    programmVec.clear();
    errorList.clear();
    for(unsigned long i=0;i<ProgrammVecTemp.size();i++){
        try{
            programmVec.push_back(new Command(&globalVaribles,int(i),ProgrammVecTemp[i]));
        }
        catch(Error *er){
            qDebug()<<er->getMessage();
            errorList.push_back((*er));

        }
    }
    emit errorOccured();
    if(errorList.size()>0){

    }
    else{
        runningCommand=0;
    }
}


/*
void  Programm::pause(vector<QString> Programm){
    if(Programm.size()==2){
        qDebug()<<"set Timer";
        QTimer::singleShot(Programm[1].toInt(), this, SLOT(ProgrammFinishedSlot()));
    }
    else{
       //printError("Error: Programm for Turn to short: Example pause 100",runningCommand);
    }
}*/
void Programm::stopJoints(){
    for(int i=0;i<globalVaribles.doubleJointMotor.size();i++){
        globalVaribles.doubleJointMotor[i]->running=false;
    }
}
/*
void Programm::setPos(vector<QString> Programm){
    if(Programm.size()<3){
       //printError("Error: Programm for Turn to short: Example  set J4 100",runningCommand);
    }
    else{
        Joint *j;
        j->setPosition(Programm[2].toInt());
    }
}*/
