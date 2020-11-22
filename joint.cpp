#include "joint.h"

Joint::Joint(int _max, int _min)
{
    setMax(_max);
    setMin(_min);
}
void Joint::setDoubleJointMotor(DoubleJointMotor *_dJM,unsigned _joint){
    dJM=_dJM;
    if(joint<2){
        joint=_joint;
        doubleJointMotorEnable=true;
    }
}

bool Joint::setPosition(int _pos){
    if(checkPosition(_pos)){
        position=_pos;
        return true;
    }
    return false;
}
bool Joint::turnPosition(int _steps){
    if(checkPosition(_steps+position)){
        target=_steps+position;
        moveJoint();
        return true;
    }
    return false;
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

void Joint::moveJoint(){
    if(target<position){
        for(int i=position;i>target;i--){
            dJM->step(joint,0);
        }
    }
    else if(target>position){
        for(int i=position;i<target;i++){
            dJM->step(joint,1);
        }
    }
}

bool Joint::checkPosition(int _pos){
    return _pos<max&&_pos>min;
}
