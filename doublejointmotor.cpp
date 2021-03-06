#include "doublejointmotor.h"

DoubleJointMotor::DoubleJointMotor(int _m1_Step,int _m1_Dir, int _m2_Step,int _m2_Dir,unsigned *_tempo,QObject *parent):
    QThread (parent)
{
    m1_Step=_m1_Step;
    m1_Dir=_m1_Dir;
    m2_Step=_m2_Step;
    m2_Dir=_m2_Dir;
    tempo=_tempo;
#ifdef RASPBERRYPI

    pinMode(m1_Step,OUTPUT);
    pinMode(m1_Dir,OUTPUT);
    pinMode(m2_Step,OUTPUT);
    pinMode(m2_Dir,OUTPUT);
#endif
}

DoubleJointMotor::~DoubleJointMotor(){

}
void DoubleJointMotor::move(int _steps,int _joint, int _direction){
    steps=_steps;
    joint=_joint;
    direction=_direction;
}
void DoubleJointMotor::run(){
    QMutex mutex;
    mutex.lock();
    running=true;
    mutex.unlock();
    for(int i=0;i<steps;i++){
        QMutex mutex;
        mutex.lock();
        if(running){
            mutex.unlock();
            step();
        }
        else{
            mutex.unlock();
            running=true;
            break;
        }
    }

    emit commandFinished();
}
void DoubleJointMotor::step(){
    emit executedStep(joint, direction);
#ifdef RASPBERRYPI
        if(joint==0){
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
        else if(joint ==1){
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
#endif
        usleep(unsigned((*tempo)));
        #ifdef RASPBERRYPI
        digitalWrite(m1_Step,LOW);
        digitalWrite(m2_Step,LOW);

#endif
        usleep(unsigned((*tempo)));
}
