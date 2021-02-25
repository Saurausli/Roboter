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
#include "operation.h"
using namespace std;



class Programm:public QObject
{
    Q_OBJECT
public:
    explicit Programm(QObject *parent= nullptr);
    ~Programm();


   /* bool checkProgramm(QString _Programm);
    void connectProgramm(bool _loop);
    void startProgramm();*/
    void compileProgram(QString arg_program);
    static vector<QString> split(QString _str, char delimiter);

signals:
    void newOutput(QString output);
    void newRunningProgram(QString programm);
    void errorOccured();
    void programmFinished();
    void newRunningCommand();
protected:
    ErrorList *errorList;
private:
    void print(QString arg_print);
    VariableSet *varSet;
    CommandList comList;
    /*
x = 1

public slots:
    void stopJoints();

protected:
    bool loop;

    GlobalVariables *globalVaribles;
    int runningCommand;
    vector<Command*> programmVec;
private:

private slots:
    void newRunningCommandSlot(int lineID);
    void ProgrammFinishedSlot();
    void gotoSlot(unsigned int line);
    void commandError(Error er);*/
};

#endif // Programm_H
