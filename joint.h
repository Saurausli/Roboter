#ifndef JOINT_H
#define JOINT_H
#include "doublejointmotor.h"
#include <QThread>

#include <QObject>
#include <QDebug>

class Joint: public QThread
{
    Q_OBJECT
public:
    explicit Joint(QString _name,int _max, int _min,QObject *parent = nullptr);
    ~Joint();
    void run();
    void setDoubleJointMotor(DoubleJointMotor *_dJM,unsigned _joint);
    int getMax();
    int getMin();

    void setMax(int _max);

    void setMin(int _min);
    bool setPosition(int _pos);
    void turnPosition(int _steps);
    QString getName();
signals:  
    void commandFinished();

private slots:
    void positionChanged(int _joint, int _direction);
private:
    void moveJoint();
    bool checkPosition(int _pos);
    int position;
    int target;
    int max;
    int min;
    QString name;
    bool doubleJointMotorEnable;
    DoubleJointMotor* dJM;
    unsigned joint;
};

#endif // JOINT_H
