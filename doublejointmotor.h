#ifndef DOUBLEJOINTMOTOR_H
#define DOUBLEJOINTMOTOR_H
//#define ARDUINOVERSION
#include <iostream>
#include<chrono>
#include<thread>
#ifdef RASPBERRYPI
    #include "wiringPi.h"
    #include <unistd.h>
#endif
using namespace std;
class DoubleJointMotor
{
public:
    DoubleJointMotor(int _m1_Step,int _m1_Dir, int _m2_Step,int _m2_Dir,unsigned* _tempo);
    void step(int _joint, int direction);


private:
    int m1_Step;
    int m1_Dir;
    int m2_Step;
    int m2_Dir;
    unsigned *tempo;
};

#endif // DOUBLEJOINTMOTOR_H
