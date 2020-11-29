#include "command.h"

Command::Command(QObject *parent) :
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
    runningCommand=0;
    connect(this,SIGNAL(commandFinished()),this,SLOT(nextCommand()));
    for(unsigned long i=0; i<joints.size();i++){

        connect(joints[i],SIGNAL(commandFinished()),this,SLOT(commandFinishedSlot()));
    }
    for(unsigned long i=0; i<doubleJointMotor.size();i++){
        connect(doubleJointMotor[i],SIGNAL(commandFinished()),this,SLOT(commandFinishedSlot()));
    }
}

Command::~Command(){


}

void Command::checkCommand(QString _command){
    commandVecTemp=split(_command,'\n');
    runningCommand=0;
    nextCommand();
}


void Command::turn(vector<QString> command){
    if(command.size()<3){
        printError("Error: Command for Turn to short: Example  turn J4 100",runningCommand);
    }
    else{
        Joint *j;

        if(command[1]=="J1"){
            j=joints[0];
        }
        else if(command[1]=="J2"){
            j=joints[1];
        }
        else{
            printError("Error: Joint not found",runningCommand);
            return;
        }

        j->turnPosition(command[2].toInt());
    }
}

void Command::setTempo(vector<QString> command){
    if(command.size()==2){
        tempo=command[1].toInt();
    }
    else{
        printError("Error: Command for Turn to short: Example tempo 100",runningCommand);
    }
    commandFinishedSlot();
}

void  Command::pause(vector<QString> command){
    if(command.size()==2){
        qDebug()<<"set Timer";
        QTimer::singleShot(command[1].toInt(), this, SLOT(commandFinishedSlot()));
    }
    else{
        printError("Error: Command for Turn to short: Example pause 100",runningCommand);
    }
}
void Command::stopJoints(){
    for(int i=0;i<doubleJointMotor.size();i++){
        doubleJointMotor[i]->running=false;
    }
}

void Command::setPos(vector<QString> command){
    if(command.size()<3){
        printError("Error: Command for Turn to short: Example  set J4 100",runningCommand);
    }
    else{
        Joint *j;

        if(command[1]=="J1"){
            j=joints[0];
        }
        else if(command[1]=="J2"){
            j=joints[1];
        }
        else{
            printError("Error: Joint not found",runningCommand);
            return;
        }

        j->setPosition(command[2].toInt());
    }
    commandFinishedSlot();
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

void Command::printError(QString respond,int line){
    printRespond(QString::number(line+1)+": "+respond);
    emit errorOccured(line);
}

void Command::printRespond(QString respond){
    qDebug()<<respond;
    emit newRespond(respond);
}

void Command::nextCommand(){
    if(runningCommand<commandVecTemp.size()){
        emit newRunningCommand(runningCommand);
        vector<QString> commandVec;
        commandVec=split(commandVecTemp[runningCommand],' ');
        if(commandVec.size()>0){
            if(commandVec[0]==TEMPO_SYNTAX){
                setTempo(commandVec);
            }
            else if(commandVec[0]==TURN_SYNTAX_NAME){
                   turn(commandVec);
               }
            else if(commandVec[0]==PAUSE_SYNTAX){
                   pause(commandVec);
               }
            else if(commandVec[0]==SET_SYNTAX_NAME){
                   setPos(commandVec);
               }
            else {
                printError("Error: Syntax unkown",runningCommand);
            }
        }
        else{
            commandFinishedSlot();
            return;
        }
    }
    else{
        if(loop){
            runningCommand=0;
            emit commandFinished();
            return;
        }
    }
}

void Command::commandFinishedSlot(){
    runningCommand++;
    emit commandFinished();
}
