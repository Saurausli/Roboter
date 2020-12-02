#include "programm.h"

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


bool Programm::checkProgramm(QString _Programm){
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
            programmVec.push_back(new Command(&globalVaribles,int(i),ProgrammVecTemp[i]));
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
        for(unsigned long i=0;i<programmVec.size();i++){
            connect(programmVec[i],SIGNAL(commandStart(int)),this,SLOT(newRunningCommandSlot(int)));
        }
    }
}

void Programm::startProgramm(){
    programmVec[0]->exec();

}
void Programm::stopJoints(){
    for(unsigned long i=0;i<globalVaribles.doubleJointMotor.size();i++){
        globalVaribles.doubleJointMotor[i]->running=false;
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

