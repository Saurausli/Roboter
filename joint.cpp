#include "joint.h"

Joint::Joint(int _max, int _min)
{
    setMax(_max);
    setMin(_min);
    position=0;
}
void Joint::setDoubleJointMotor(DoubleJointMotor *_dJM,unsigned _joint){
    dJM=_dJM;
    if(_joint<2){
        joint=_joint;
        doubleJointMotorEnable=true;
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
bool Joint::turnPosition(int _steps){
    cout<<"turn Position: lets turn position "<<_steps<<" "<<position<<endl;
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
        while(target<position){
            dJM->step(joint,0);
            position--;
        }
    }
    else if(target>position){
        while(target>position){
            dJM->step(joint,1);
            position++;
        }
    }
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
