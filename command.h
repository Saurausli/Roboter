#ifndef COMMAND_H
#define COMMAND_H

#include <QObject>
#include <vector>
#include <QString>
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
    void checkCommand(QString _command);


    void setPos(vector<QString> command,int syntaxInedx);
    void turn(vector<QString> command,int syntaxInedx);
    void setTempo(vector<QString> command,int syntaxInedx);
signals:
    void newRespond(QString output);
    void newRunningProgramm(QString programm);
    void newRunningCommand(int commandLine);

private:
   vector<QString> split(QString _str, char delimiter);
   vector<Joint> joints;
   unsigned tempo;
   QString output;
   vector<DoubleJointMotor> doubleJointMotor;
   void printRespond(QString respond);


};

#endif // COMMAND_H
