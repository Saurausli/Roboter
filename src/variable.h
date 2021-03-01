#ifndef VARIABLE_H
#define VARIABLE_H

#include <QObject>
#include <QDebug>
#include "error.h"

#define VariableSyntaxInteger "int"
#define VariableSyntaxBool "bool"

#define BoolValueTrue "true"
#define BoolValueFalse "false"

class Variable;

typedef std::vector<Variable*> VariableSet;

enum VariableType{
    unknown,
    Integer,
    Boolean,
};

class Variable
{
public:
    Variable(VariableType arg_type,QString arg_name,QString arg_value);
    //Variable(VariableType arg_type,QString arg_name,QString arg_value);
    Variable(VariableType arg_type,QString arg_name);
    void setValue(QString arg_value);
    void setValue(int arg_value);
    void setValue(bool arg_value);
    void setType(VariableType arg_type);
    void define();
    int getValuetoInt();
    bool getValuetoBool();
    QString getValue();
    QString getName();
    VariableType getVariableType();
    static bool checkIfIsVariableType(QString arg_type);
    static void checkNumber(QString number);
    static VariableType getVariableTypeFromString(QString arg_name);
    static void getVariableTypeFromString(QString arg_name, VariableType &var_Type,bool errorMessage=false);
    static vector<QString> getVariableTypeSyntax();
    static bool toBool(QString arg_value);
    static void checkBool(QString arg_value);
    static QString getVariableTypeName(VariableType arg_type);
private:
    QString value;
    VariableType type;
    QString name;
    bool defined;
    bool writeable;
    void varSetup(VariableType arg_type,QString arg_name);
};

#endif // VARIABLE_H
