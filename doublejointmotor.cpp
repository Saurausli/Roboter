#include "doublejointmotor.h"

DoubleJointMotor::DoubleJointMotor(int _m1_Step,int _m1_Dir, int _m2_Step,int _m2_Dir,unsigned *_tempo)
{
    m1_Step=_m1_Step;
    m1_Dir=_m1_Dir;
    m2_Step=_m2_Step;
    m2_Dir=_m2_Dir;
    tempo=_tempo;
#ifdef RASPBERRYPI
    wiringPiSetup();
    pinMode(m1_Step,OUTPUT);
    pinMode(m1_Dir,OUTPUT);
    pinMode(m2_Step,OUTPUT);
    pinMode(m2_Dir,OUTPUT);
#endif
}
void DoubleJointMotor::step(int _joint, int direction){
#ifdef RASPBERRYPI
        if(_joint==0){
            if(direction==0){
                digitalWrite(m1_Dir,HIGH);
                digitalWrite(m2_Dir,LOW);
                //cout<<"J1 +"<<endl;
            }
            else {
                digitalWrite(m1_Dir,LOW);
                digitalWrite(m2_Dir,HIGH);
                //cout<<"J1 -"<<endl;
            }
        }
        else if(_joint ==1){
            if(direction==0){
                digitalWrite(m1_Dir,HIGH);
                digitalWrite(m2_Dir,HIGH);
                //cout<<"J2 +"<<endl;
            }
            else {
                digitalWrite(m1_Dir,LOW);
                digitalWrite(m2_Dir,LOW);
                //cout<<"J2 -"<<endl;
                }
        }
        digitalWrite(m1_Step,HIGH);
        digitalWrite(m2_Step,HIGH);
        usleep(unsigned((*tempo)));
        digitalWrite(m1_Step,LOW);
        digitalWrite(m2_Step,LOW);
        usleep(unsigned((*tempo)));
#endif
}
