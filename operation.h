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

enum Task{
    defineVariable,
    calculet
};

enum Operator{
    none,
    plus,
    minus,
    overwrite
};

class Operation
{
public:
    Operation(VariableSet *arg_varSet,vector<QString> arg_operation); //for Arithmetic
    Operation(VariableSet *arg_varSet,QString arg_name, QString arg_type);   //for defines
    Operation(VariableSet *arg_varSet,QString arg_name, VariableType arg_type);   //for defines
    Operation(VariableSet *arg_varSet,QString arg_name, VariableType arg_type,QVariant arg_startValue);   //for defines
    void exec();
    Variable* getResult();
    void setResultVariable(Variable *arg_var);
    static vector<QString> getOperatorSyntax();
    static Operator getOperator(QString arg_operatorName);
    static int getProvVariableId(VariableSet *arg_varSet, QString arg_name);
    static bool isOperator(QString &arg_name);
    static bool isNumber(QString &arg_string);
private:
    //void arithmicSetup(vector<Variable*> arg_VarVec,Variable* arg_result ,Operator arg_Op);
    void setupDefine(VariableSet *arg_varSet,QString arg_name, VariableType arg_type);

    void checkLength(vector<QString> &com,unsigned long len);
    void checkMinLength(vector<QString> &com,unsigned long len);
    void checkMaxLength(vector<QString> &com,unsigned long len);
    void checkMinMaxLength(vector<QString> &com,unsigned long min,unsigned long max);
    void checkAllVarType(vector<QString> &arg_vec, VariableType arg_typ);
    void checkVarType(Variable* arg_var, VariableType arg_typ);
    void checkIfVarNameFree(QString arg_name);
    void checkVarExist(QString arg_name);

    VariableSet *varSet;
    Task task;

    QString varName;
    VariableType varType;
    QVariant startValue;
    vector<QString> operation;
    Variable *result;
};

#endif // OPERATION_H
