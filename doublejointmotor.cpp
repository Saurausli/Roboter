#include "doublejointmotor.h"

DoubleJointMotor::DoubleJointMotor(unsigned _m1_Step,unsigned _m1_Dir, unsigned _m2_Step,unsigned _m2_Dir,unsigned *_tempo)
{
    m1_Step=_m1_Step;
    m1_Dir=_m1_Dir;
    m2_Step=_m2_Step;
    m2_Dir=_m2_Dir;
    tempo=_tempo;
#ifdef ARDUINOVERSION
    pinMode(m1_Step,OUTPUT;
    pinMode(m1_Dir,OUTPUT;
    pinMode(m2_Step,INTPUT;
    pinMode(m2_Dir,INPUT);
#endif
}
void DoubleJointMotor::step(unsigned _joint, unsigned direction){
#ifdef ARDUINOVERSION
        if(_joint){
            if(direction==0){
                digitalWrite(m1_Dir,HIGH);
                digitalWrite(m2_Dir,LOW);}
            else {
                digitalWrite(m2_Dir,HIGH);
                digitalWrite(m1_Dir,LOW);}
            }
        }
        else{
            if(direction==0){
                digitalWrite(m1_Dir,HIGH);
                digitalWrite(m2_Dir,HIGH);}
            else {
                digitalWrite(m2_Dir,LOW);
                digitalWrite(m1_Dir,LOW);}
            }
        }

        digitalWrite(m1_Step,HIGH);
        digitalWrite(m2_Step,HIGH);
        delayMicroseconds(tempo);
        digitalWrite(m1_Step,LOW);
        digitalWrite(m2_Step,LOW);
        delayMicroseconds(tempo);
#endif
}
