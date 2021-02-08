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
    bool checkProgramm(QString _Programm);
    void connectProgramm(bool _loop);
    void startProgramm();
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
    GlobalVariables *globalVaribles;
    int runningCommand;
    vector<Command*> programmVec;
private:

private slots:
    void newRunningCommandSlot(int lineID);
    void ProgrammFinishedSlot();
    void gotoSlot(unsigned int line);
    void commandError(Error er);
};

#endif // Programm_H