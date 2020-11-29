#ifndef COMMAND_H
#define COMMAND_H

#include <QObject>
#include <vector>
#include <QString>
#include <sstream>
#include <iostream>
#include <QTimer>
#include "joint.h"
#include "doublejointmotor.h"
#include <QDebug>

#define TURN_SYNTAX_NAME "turn"

#define SET_SYNTAX_NAME "set"

#define TEMPO_SYNTAX "tempo"

#define PAUSE_SYNTAX "pause"

using namespace std;

class Command:protected QObject
{
    Q_OBJECT
public:
    explicit Command(QObject *parent= nullptr);
    ~Command();
    void checkCommand(QString _command);
    void setPos(vector<QString> command);
    void turn(vector<QString> command);
    void setTempo(vector<QString> command);
    void pause(vector<QString> command);
signals:
    void newRespond(QString output);
    void newRunningProgramm(QString programm);
    void newRunningCommand(int commandLine);
    void errorOccured(int line);
    void commandFinished();

public slots:
    void stopJoints();
protected:
    bool loop;

private:
   vector<QString> split(QString _str, char delimiter);
   vector<Joint *> joints;
   unsigned tempo;

   vector<QString> commandVecTemp;
   vector<DoubleJointMotor *> doubleJointMotor;
   void printError(QString respond,int line);
   void printRespond(QString respond);
   int runningCommand;

private slots:
   void nextCommand();
   void commandFinishedSlot();
};

#endif // COMMAND_H
