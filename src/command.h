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

#define Bracket_Opening "{"
#define Bracket_Closing "}"


enum BasicCommand{
    Empty,
    Set,
    While,
    If,
    ClosingBracket,
    Define
};

using namespace std;


class Command;

class Bracket{
    public:
        Bracket (Command *arg_startOperation);
        bool isClosed();
        void close(Command *arg_endOperation);
        Command *getEndOperation();
        Command *getStartOperation();
    private:
        Command *startOperation;
        Command *endOperation;
        bool closed=false;
};

typedef std::vector<Bracket*> BracketList;

typedef std::vector<Command*> CommandList;

class Command:public QObject
{
    Q_OBJECT
    public:
        explicit Command(VariableSet *arg_varSet,BracketList *arg_brackets,vector<QString> arg_command, QObject *parent= nullptr);
        ~Command();
        void checkIfAllBracketsClosed();
        BasicCommand getBasicCommand();
        static QVector<QString> getBasicCommandName();
        static BasicCommand getBasicCommandFromString(QString arg_name);
    public slots:
        void exec();
        void toNextLine();

    signals:
        void nextLine();
        void toEndBracket();
private:
    VariableSet *varSet;
    BasicCommand bcommand;
    OperationList commandOperation;
    Variable* defineVar;
    BracketList* brackets;
    vector<QString> command;

    void checkIfBracketsOpen(); 
    Bracket* getOpeningBracket();

    bool checkIfIsBasicCommand(QString arg_name);
    void checkString(QString &arg_currentString,QString arg_expectedString);
    void setupBracktHead(QString arg_name);
  };


#endif
