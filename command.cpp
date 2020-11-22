#include "command.h"

Command::Command()
{
    DoubleJointMotor dJ(7,8,9,10,&tempo);
    doubleJointMotor.push_back(dJ);
    joints.push_back(Joint(3680,-3680));
    joints[joints.size()-1].setDoubleJointMotor(&doubleJointMotor[0],0);
    joints.push_back(Joint(1300,-1300));
    joints[joints.size()-1].setDoubleJointMotor(&doubleJointMotor[0],1);
    tempo=100;

}

void Command::checkCommand(string _command){
    vector<string> commandVec;
    commandVec=split(_command,' ');
    for(int i=0;i<commandVec.size();i++){
        if(commandVec[i]==SET_SYNTAX_NAME){

        }
        else if(commandVec[i]==TURN_SYNTAX_NAME){
            turn(commandVec,i);
        }
        else if(commandVec[i]==TEMPO_SYNTAX){
            if(commandVec.size()>i){
                tempo=std::stoi(commandVec[i+1]);
                printRespond("new Tempo "+std::to_string(tempo));
            }
        }
    }
}

void Command::turn(vector<string> command,int syntaxInedx){
    if(command.size()<syntaxInedx+2){
        printRespond("Error: Command for Turn to short: Example J4 turn 100 steps left");
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
        j->turnPosition(std::stoi(command[syntaxInedx+1]));
    }
}

void Command::setPos(vector<string> command,int syntaxInedx){

}

vector<string> Command::split(string str, char delimiter) {
  vector<string> internal;
  stringstream ss(str); // Turn the string into a stream.
  string tok;

  while(getline(ss, tok, delimiter)) {
    internal.push_back(tok);
  }

  return internal;
}

void Command::printRespond(string respond){
#ifdef ARDUINOVERSION
    Serial.println(respond);
#else
     cout<<respond<<endl;
#endif
}
