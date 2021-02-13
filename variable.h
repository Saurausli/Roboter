#ifndef VARIABLE_H
#define VARIABLE_H

#include <QObject>
#include <QDebug>
#include "error.h"

#define VariableSyntaxInteger "int"

class Variable;



enum VariableType{
    unknown,
    integer
};

struct VariableSet{
    vector<QString> names;
    vector<VariableType> type;
    vector<Variable*> variable;
};

class Variable
{
public:
    Variable(VariableType arg_type,QString arg_name,QVariant arg_value);
    Variable(VariableType arg_type,QString arg_name);

    void setValue(QString arg_value);
    void setValue(int arg_value);
    int getValuetoInt();
    QString getName();
    VariableType getVariableType();
    static bool checkIfIsVariableType(QString arg_type);
    static void checkNumber(QString number);
    static void getVariableTypeFromString(QString arg_name, VariableType &var_Type,bool errorMessage=false);
private:
    QVariant *value;
    VariableType type;
    QString name;
    void varSetup(VariableType arg_type,QString arg_name);
};

#endif // VARIABLE_H
