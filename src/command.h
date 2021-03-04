#ifndef COMMAND_H
#define COMMAND_H

#include <QObject>
#include <sstream>
#include <iostream>
#include <QTimer>
#include "error.h"
#include "joint.h"
#include "operation.h"

#define Basic_Command_While "while"
#define Basic_Command_If "if"
#define Basic_Command_Else "else"
#define Basic_Command_ElseIf "elseif"
#define Basic_Command_ElseIf_Textversion "else if"
#define Basic_Command_Main "main"




enum BasicCommand{
    Empty,
    Set,
    While,
    If,
    Else,
    ElseIf,
    ClosingBracket,
    Define,
    Main,
};

using namespace std;


class Command;
class IfElse;
class Bracket;
typedef std::vector<IfElse*> IfElseList;
typedef std::vector<Command*> CommandList;
typedef std::vector<Bracket*> BracketList;

class Bracket{
    public:
        Bracket (Command *arg_startOperation,VariableSet arg_varSet);
        bool isClosed();
        void close(Command *arg_endOperation);
        Command *getEndOperation();
        Command *getStartOperation();
        VariableSet varSet;
    private:
        Command *startOperation;
        Command *endOperation;
        bool closed=false;
};

class IfElse{
    public:
        IfElse (Command *arg_ifOperation,Bracket *arg_parentBracket);
        bool isClosed();
        void close();
        void close(Command *arg_elseOperation);
        Command *getElseOperation();
        Command *getIfOperation();
        Bracket *getBracket();
        bool isElseDefined();
        void addElseIf(Command *arg_elseIf);
        CommandList *getElseIfList();
        Command *getLastIf();
        Command *getPreviousIf(Command *arg_elseIf);
    private:
        Command *ifOperation;
        Command *elseOperation;
        CommandList elseIf;
        bool closed=false;
        bool elseDefined=false;
        Bracket *parentBracket;
};





struct ProgramData{
    BracketList bracketList;
    IfElseList ifElseList;
    CommandList CommandList;
    VariableSet varSet;
};

class Command:public QObject
{
    Q_OBJECT
    public:
        explicit Command(ProgramData *arg_programData,vector<QString> arg_command, QObject *parent= nullptr);
        ~Command();
        void checkIfAllBracketsClosed();
        BasicCommand getBasicCommand();
        static QVector<QString> getBasicCommandName();
        static BasicCommand getBasicCommandFromString(QString arg_name);
        OperationList* getOperationList();
    public slots:
        void exec();
        void toNextLine();

    signals:
        void nextLine();
        void toEndBracket();
private:
    ProgramData *programData;
    BasicCommand bcommand;
    OperationList commandOperation;
    Variable* defineVar;
    vector<QString> command;
    Command* parentIf;
    Bracket* parentBracket;
    void checkIfBracketsOpen(); 
    Bracket* getBracket();
    Bracket* getParentBracket();
    IfElse* getOpenIfElse();
    IfElse* getIfElseFromElse();
    IfElse* getIfElseFromElseIf();
    bool getPreviousIfResult();
    bool checkIfIsBasicCommand(QString arg_name);
    int getIntArgument(BasicCommand arg_bcommand);
    void checkString(QString &arg_currentString,QString arg_expectedString);
    void setupBracktHead(QString arg_name);
  };


#endif
