#ifndef JOINT_H
#define JOINT_H
#include "doublejointmotor.h"

class Joint
{
public:
    Joint(int _max, int _min);
    void setDoubleJointMotor(DoubleJointMotor *_dJM,unsigned _joint);
    bool setPosition(int _pos);
    bool turnPosition(int _steps);

    void setMax(int _max);
    int getMax();

    void setMin(int _min);
    int getMin();
private:
    void moveJoint();
    bool checkPosition(int _pos);
    int position;
    int target;
    int max;
    int min;
    bool doubleJointMotorEnable;
    DoubleJointMotor* dJM;
    unsigned joint;
};

#endif // JOINT_H
