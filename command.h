#ifndef COMMAND_H
#define COMMAND_H

#include <QObject>
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

class Command:public QObject
{
    Q_OBJECT
public:
    explicit Command(QObject *parent= nullptr);
    ~Command();
    void checkCommand(string _command);


    void setPos(vector<string> command,int syntaxInedx);
    void turn(vector<string> command,int syntaxInedx);

signals:
    void newRespond(QString output);

private:
   vector<string> split(string str, char delimiter);
   vector<Joint> joints;
   unsigned tempo;
   string output;
   vector<DoubleJointMotor> doubleJointMotor;
   void printRespond(string respond);


};

#endif // COMMAND_H
