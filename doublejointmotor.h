#ifndef DOUBLEJOINTMOTOR_H
#define DOUBLEJOINTMOTOR_H
//#define ARDUINOVERSION

#ifdef ARDUINOVERSION
#include "arduino.h"
#endif
class DoubleJointMotor
{
public:
    DoubleJointMotor(unsigned _m1_Step,unsigned _m1_Dir, unsigned _m2_Step,unsigned _m2_Dir,unsigned* _tempo);
    void step(unsigned _joint, unsigned direction);


private:
    unsigned m1_Step;
    unsigned m1_Dir;
    unsigned m2_Step;
    unsigned m2_Dir;
    unsigned *tempo;
};

#endif // DOUBLEJOINTMOTOR_H
