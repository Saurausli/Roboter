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

#define DEF_LOOPSTART_SYNTAX "loopStart"

#define DEF_LOOPEND "loopEnd"

#define DEF_COMMENT_SYNTAX "//"
enum Function{
    error,
    empty,
    turn,
    set,
    tempo,
    pause,
    label,
    gotoRobo,
    loopStart,
    loopEnd,
    comment
};

struct Loop{
    unsigned int startLine;
    unsigned int endLine;
    unsigned int loopCount=0;
    unsigned int starLoopCount;
    bool defined=false;
    bool endlessLoop=false;
};

struct Label{
    unsigned int gotoLine;
    QString labelName;
};

struct GlobalVariables{
    vector<Joint *> joints;
    unsigned int tempo;
    vector<DoubleJointMotor *> doubleJointMotor;
    vector<Label> lableVec;
    vector<Loop> loopVec;
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
        void executeError(Error e);
    private:
        unsigned int programmLine;
        QString command;
        Function function;
        GlobalVariables *globalVariables;
        Joint *joint;
        void getLable(QString name,unsigned int &_line);
        void setJoint(QString name);
        void checkNumber(QString number);
        bool checkWordBeginnig(QString word,QString beginning);
        void checkLength(vector<QString> *com,unsigned long len);
    private slots:
        void commandFinishedSlot();
};



#endif
