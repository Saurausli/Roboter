#ifndef DOUBLEJOINTMOTOR_H
#define DOUBLEJOINTMOTOR_H
//#define ARDUINOVERSION
#include <iostream>
#include<chrono>
#include <QDebug>
#include <QThread>
#include <QObject>
#include <QMutex>
#ifdef RASPBERRYPI
    #include "wiringPi.h"
    #include <unistd.h>
#endif
using namespace std;

class DoubleJointMotor: public QThread
{
    Q_OBJECT
public:
    explicit DoubleJointMotor(QString _name, int _m1_Step,int _m1_Dir, int _m2_Step,int _m2_Dir,unsigned* _tempo, QObject *parent = nullptr);
    ~DoubleJointMotor();
    void move(int _steps,int _joint, int _direction);

    void run();
    bool running;
    QString getName();
signals:
    void commandFinished();
    void executedStep(int _joint, int _direction);
private:
    int m1_Step;
    int m1_Dir;
    int m2_Step;
    int m2_Dir;
    int joint;
    int steps;
    int direction;
    QString name;
    unsigned *tempo;
    void step();
};

#endif // DOUBLEJOINTMOTOR_H
