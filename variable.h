#ifndef VARIABLE_H
#define VARIABLE_H

#include <QObject>
#include <QDebug>
#include "error.h"

#define VariableSyntaxInteger "int"

class Variable;

typedef std::vector<Variable*> VariableSet;

enum VariableType{
    unknown,
    integer
};

class Variable
{
public:
    Variable(VariableType arg_type,QString arg_name,QString arg_value);
    Variable(VariableType arg_type,QString arg_name);
    void setValue(QString arg_value);
    void setValue(int arg_value);
    void define();
    int getValuetoInt();
    QString getName();
    VariableType getVariableType();
    static bool checkIfIsVariableType(QString arg_type);
    static void checkNumber(QString number);
    static void getVariableTypeFromString(QString arg_name, VariableType &var_Type,bool errorMessage=false);
    static vector<QString> getVariableTypeSyntax();
private:
    QString value;
    VariableType type;
    QString name;
    bool defined;
    void varSetup(VariableType arg_type,QString arg_name);
};

#endif // VARIABLE_H
