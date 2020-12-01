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
#include <fstream>

using namespace std;



class Programm:public QObject
{
    Q_OBJECT
public:
    explicit Programm(QObject *parent= nullptr);
    ~Programm();
    void checkProgramm(QString _Programm);
signals:
    void newRespond(QString output);
    void newRunningProgramm(QString programm);
    void newRunningCommand(int commandLine);
    void errorOccured();
    void ProgrammFinished();

public slots:
    void stopJoints();

protected:
    bool loop;
    ErrorList errorList;
    GlobalVariables globalVaribles;
    int runningCommand;
private:
   vector<Command*> programmVec;
private slots:

};

#endif // Programm_H
