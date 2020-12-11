#include "joint.h"

Joint::Joint(QString _name,int _max, int _min,QObject *parent):
    QThread (parent)
{
    setMax(_max);
    setMin(_min);
    name=_name;
    position=0;
}

Joint::~Joint(){

}

void Joint::run(){

    moveJoint();
    emit commandFinished();
}

void Joint::setDoubleJointMotor(DoubleJointMotor *_dJM,unsigned _joint){
    dJM=_dJM;
    if(_joint<2){
        joint=_joint;
        doubleJointMotorEnable=true;
        connect(dJM,SIGNAL(executedStep(int,int)),this,SLOT(positionChanged(int,int)));
    }
}

bool Joint::setPosition(int _pos){
    if(checkPosition(_pos)){
        position=_pos;
        return true;
    }
    else{
        cout<<"position will overshot maximum"<<endl;
    }
    return false;
}

void Joint::turnPosition(int _steps){
    if(checkPosition(_steps+position)){
        target=_steps+position;
        moveJoint();
    }

}

QString Joint::getName(){
    return name;
}

void Joint::setMax(int _max){
    max=_max;
}
int Joint::getMax(){
    return max;
}

void Joint::setMin(int _min){
    min=_min;
}
int Joint::getMin(){
    return min;
}
void Joint::positionChanged(int _joint, int _direction){
    if(_joint==joint){
        if(_direction==0){
            position--;
        }
        else if(_direction==1){
            position++;
        }
        if(!checkPosition(position)){
            dJM->running=false;
        }
    }
}

void Joint::moveJoint(){
    connect(dJM,SIGNAL(commandFinished()),this,SLOT(turnFinished()));
    if(target<position){

            dJM->move(position-target,joint,0);
    }
    else if(target>position){
            dJM->move(target-position,joint,1);
    }
    dJM->start();
}

bool Joint::checkPosition(int _pos){
    if(_pos>max){
        cout<<"Error: position "<<_pos<<" to bigger then max "<<max<<endl;
        return false;
    }
    if(_pos<min){
        cout<<"Error: position "<<_pos<<" to lower then min "<<min<<endl;
        return false;
    }
    return true;
}

void Joint::turnFinished(){
    disconnect(dJM,SIGNAL(commandFinished()),this,SLOT(turnFinished()));
    emit commandFinished();
}
