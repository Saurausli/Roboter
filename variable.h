#ifndef VARIABLE_H
#define VARIABLE_H

#include <QObject>
#include <QDebug>
#include "error.h"
enum VariableType{
    integer
};

class Variable
{
public:
    Variable(VariableType arg_type,QString arg_name);
    Variable(VariableType arg_type);
    void setValue(QString arg_value);
    int getValuetoInt();
    QString getName();
    VariableType getVariableType();
    static void checkNumber(QString number);
private:
    QVariant *value;
    VariableType type;
    QString name;
};

#endif // VARIABLE_H
