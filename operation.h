#ifndef OPERATION_H
#define OPERATION_H

#include <QObject>
#include "variable.h"
#include <vector>
#include <iostream>


#define OperatorSyntaxPlus "+"
#define OperatorSyntaxMinus "-"

using namespace std;

enum Operator{
    none,
    plus,
    minus,
};

class Operation
{
public:
    Operation(vector<Variable*> arg_VarVec,Operator arg_Op);
    void calc();
    Variable* getResult();
    static Operator getOperator(QString arg_operatorName);
private:
    void checkLength(vector<Variable*>  &com,unsigned long len);
    void checkMinLength(vector<Variable*>  &com,unsigned long len);
    void checkMaxLength(vector<Variable*>  &com,unsigned long len);
    void checkMinMaxLength(vector<Variable*>  &com,unsigned long min,unsigned long max);
    vector<Variable*> varVec;
    Operator operatorSymbol;
    Variable *result;
};

#endif // OPERATION_H
