#ifndef OPERATION_H
#define OPERATION_H

#include <QObject>
#include "variable.h"
#include <vector>
#include <iostream>


#define OperatorSyntaxPlus "+"
#define OperatorSyntaxMinus "-"

#define OperatorSyntaxEqual "="

using namespace std;

enum Operator{
    none,
    defineVariable,
    plus,
    minus,
    overwrite
};

class Operation
{
public:
    Operation(vector<Variable*> arg_VarVec,Variable* arg_result ,Operator arg_Op); //for Arithmetic
    Operation(Variable* arg_Var,Variable* arg_result); //for Arithmetic
    Operation(Variable* arg_result ,Operator arg_Op,vector<Variable*> *arg_defineVec);   //for defines
    void exec();
    Variable* getResult();
    void setResultVariable(Variable *arg_var);
    static Operator getOperator(QString arg_operatorName);
private:
    void arithmicSetup(vector<Variable*> arg_VarVec,Variable* arg_result ,Operator arg_Op);
    void checkLength(vector<Variable*>  &com,unsigned long len);
    void checkMinLength(vector<Variable*>  &com,unsigned long len);
    void checkMaxLength(vector<Variable*>  &com,unsigned long len);
    void checkMinMaxLength(vector<Variable*>  &com,unsigned long min,unsigned long max);
    void checkAllVarType(vector<Variable*> arg_vec, VariableType arg_typ);
    void checkVarType(Variable* arg_var, VariableType arg_typ);
    vector<Variable*> varVec;
    vector<Variable*> *defineVec;
    Operator operatorSymbol;
    Variable *result;
};

#endif // OPERATION_H
