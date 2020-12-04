#ifndef COMMAND_H
#define COMMAND_H

#include <QObject>
#include <sstream>
#include <iostream>
#include <QTimer>
#include "error.h"
#include "joint.h"



#define TURN_SYNTAX_NAME "turn"

#define SET_SYNTAX_NAME "set"

#define TEMPO_SYNTAX "tempo"

#define PAUSE_SYNTAX "pause"

#define LABLE_SYNTAX "label:"

#define GOTO_SYNTAX "goto"

#define DEF_DOUBLEMOTOR_SYNTAX "Doublemotor"

#define DEF_JOINT_SYNTAX "Joint"
enum Function{
    error   =-1,
    empty   =0,
    turn    =1,
    set     =2,
    tempo   =3,
    pause   =4,
    label   =5,
    gotoRobo     =6
};

struct Label{
    unsigned int gotoLine;
    QString labelName;
};

struct GlobalVariables{
    vector<Joint *> joints;
    unsigned int tempo;
    vector<DoubleJointMotor *> doubleJointMotor;
    vector<Label *> lableVec;
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
        unsigned int getLine();
        static vector<QString> split(QString _str, char delimiter);
    public slots:
        void exec();

    signals:
        void commandStart(int line);
        void commandFinished();
        void gotoCommand(unsigned int line);
    private:
        unsigned int programmLine;
        QString command;
        Function function;
        GlobalVariables *globalVariables;
        Joint *joint;
        int getLable(QString name);
        void setJoint(QString name);
        void checkNumber(QString number);
        void checkLength(vector<QString> *com,unsigned long len);
    private slots:
        void commandFinishedSlot();
};



#endif
