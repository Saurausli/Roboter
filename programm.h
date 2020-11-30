#ifndef Programm_H
#define Programm_H

#include <QObject>
#include <vector>
#include <QString>
#include <sstream>
#include <iostream>
#include <QTimer>
#include "joint.h"
#include "doublejointmotor.h"
#include <QDebug>
#include "error.h"
#include "command.h"

#define TURN_SYNTAX_NAME "turn"

#define SET_SYNTAX_NAME "set"

#define TEMPO_SYNTAX "tempo"

#define PAUSE_SYNTAX "pause"

using namespace std;



class Programm:protected QObject
{
    Q_OBJECT
public:
    explicit Programm(QObject *parent= nullptr);
    ~Programm();
    void checkProgramm(QString _Programm);
    void setPos(vector<QString> Programm);
    void turn(vector<QString> Programm);
    void setTempo(vector<QString> Programm);
    void pause(vector<QString> Programm);
signals:
    void newRespond(QString output);
    void newRunningProgramm(QString programm);
    void newRunningCommand(int ProgrammLine);
    void errorOccured(QVector<int> line,QVector<QString message);
    void ProgrammFinished();

public slots:
    void stopJoints();
protected:
    bool loop;

private:
   vector<QString> split(QString _str, char delimiter);
   vector<Joint *> joints;
   unsigned tempo;
   ErrorList errorList;
   vector<Command> programmVec;
   vector<DoubleJointMotor *> doubleJointMotor;
   void printError();
   int runningCommand;

private slots:
   void nextProgramm();
   void ProgrammFinishedSlot();
};

#endif // Programm_H
