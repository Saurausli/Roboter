#ifndef COMMAND_H
#define COMMAND_H

#include <QObject>
#include <sstream>
#include <iostream>
#include "error.h"
#include "joint.h"


#define TURN_SYNTAX_NAME "turn"

#define SET_SYNTAX_NAME "set"

#define TEMPO_SYNTAX "tempo"

#define PAUSE_SYNTAX "pause"

enum Function{
    error   =-1,
    empty   =0,
    turn    =1,
    set     =2,
    tempo   =3,
    pause   =4
};

struct GlobalVariables{
    vector<Joint *> joints;
    unsigned tempo;
    vector<DoubleJointMotor *> doubleJointMotor;
};

using namespace std;



class Command:public QObject
{

    Q_OBJECT
    public:
        explicit Command(GlobalVariables* _globalVariables,int _programmLine, QString _command, QObject *parent= nullptr);
        ~Command();
        void checkCommand();
        Function getFunction();
        void exec();
        static vector<QString> split(QString _str, char delimiter);
    private:
        unsigned int programmLine;
        QString command;
        Function function;
        GlobalVariables *globalVariables;
        Joint *joint;
        void setJoint(QString name);
        void checkNumber(QString number);
        void checkLength(vector<QString> *com,unsigned long len);
};



#endif
