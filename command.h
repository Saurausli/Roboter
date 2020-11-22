#ifndef COMMAND_H
#define COMMAND_H


#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include "joint.h"
#include "doublejointmotor.h"

#define TURN_SYNTAX_NAME "turn"

#define SET_SYNTAX_NAME "set"

#define TEMPO_SYNTAX "tempo"

using namespace std;

class Command
{
public:
    Command();
    void checkCommand(string _command);


    void setPos(vector<string> command,int syntaxInedx);
    void turn(vector<string> command,int syntaxInedx);

private:
   vector<string> split(string str, char delimiter);
   vector<Joint> joints;
   unsigned tempo;
   vector<DoubleJointMotor> doubleJointMotor;
   void printRespond(string respond);
};

#endif // COMMAND_H
