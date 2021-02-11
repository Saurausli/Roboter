#ifndef VARIABLE_H
#define VARIABLE_H

#include <QObject>
#include <QDebug>
#include "error.h"

#define VariableSyntaxInteger "int"
enum VariableType{
    integer
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
    static void checkNumber(QString number);
private:
    QVariant *value;
    VariableType type;
    QString name;
    void varSetup(VariableType arg_type,QString arg_name);
};

#endif // VARIABLE_H
