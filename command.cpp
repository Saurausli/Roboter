#include "command.h"

Command::Command(QObject *parent) :
    QObject(parent){
    DoubleJointMotor dJ(22,23,24,25,&tempo);
    doubleJointMotor.push_back(dJ);
    joints.push_back(Joint(3680,-3680));
    joints[joints.size()-1].setDoubleJointMotor(&doubleJointMotor[0],0);
    joints.push_back(Joint(4000,-4000));
    joints[joints.size()-1].setDoubleJointMotor(&doubleJointMotor[0],1);
    tempo=100;

}
Command::~Command(){


}

void Command::checkCommand(QString _command){

    vector<vector<QString>> commandVec;
    vector<QString> commandVecTemp;
    commandVecTemp=split(_command,'\n');
    for(int i=0;i<commandVecTemp.size();i++){
        commandVec.push_back(split(commandVecTemp[i],' '));
    }
    for(int i=0;i<commandVec.size();i++){
        for(int j=0;j<commandVec[i].size();j++){
            if(commandVec[i][j]==SET_SYNTAX_NAME){
                return;
            }
            else if(commandVec[i][j]==TURN_SYNTAX_NAME){
                turn(commandVec[i],i);
                return;
            }
            else if(commandVec[i][j]==TEMPO_SYNTAX){
                setTempo(commandVec[i],i);
                return;
            }
        }
    }
    printRespond("Error: Syntax unkown");
}

void Command::turn(vector<QString> command,int syntaxInedx){
    emit newRunningCommand(syntaxInedx);
    if(command.size()<syntaxInedx+2){
        printRespond("Error: Command for Turn to short: Example J4 turn 100");
    }
    else{
        Joint *j;
        if(command[0]=="J1"){
            j=&joints[0];
        }
        else if(command[0]=="J2"){
            j=&joints[1];
        }
        else{
            printRespond("Error: Joint not found");
            return;
        }
        j->turnPosition(command[syntaxInedx+1].toInt());
    }
}

void Command::setTempo(vector<QString> command,int syntaxInedx){
    emit newRunningCommand(syntaxInedx);
    if(command.size()>syntaxInedx){
        tempo=command[syntaxInedx+1].toInt();
        printRespond("new Tempo "+QString::number(tempo));
    }
}

void Command::setPos(vector<QString> command,int syntaxInedx){

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

void Command::printRespond(QString respond){
    output=respond+'\n'+output;
    emit newRespond(output);
}
