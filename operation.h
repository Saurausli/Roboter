#ifndef OPERATION_H
#define OPERATION_H

#include <QObject>

#include <vector>
#include <iostream>
#include "suboperation.h"



struct subOperationStruct{
    int start=-1;
    int end=-1;
    vector<QString> op;
};

enum Task{
    defineVariable,
    calculet
};

class Operation
{
public:
    Operation(VariableSet *arg_varSet,vector<QString> arg_operation); //for Arithmetic
    Operation(VariableSet *arg_varSet,vector<QString> arg_operation, Variable *arg_res); //for Arithmetic

    Operation(VariableSet *arg_varSet,QString arg_name, QString arg_type);   //for defines
    Operation(VariableSet *arg_varSet,QString arg_name, VariableType arg_type);   //for defines
    Operation(VariableSet *arg_varSet,QString arg_name, VariableType arg_type,QString arg_startValue);   //for defines
    void exec();
    Variable* getResult();
    void setResultVariable(Variable *arg_var);

private:
    //void arithmicSetup(vector<Variable*> arg_VarVec,Variable* arg_result ,Operator arg_Op);
    void setupDefine(VariableSet *arg_varSet,QString arg_name, VariableType arg_type,QString arg_startValue);
    void setupCalc(VariableSet *arg_varSet,vector<QString> arg_operation);

    void checkLength(vector<QString> &com,unsigned long len);
    void checkMinLength(vector<QString> &com,unsigned long len);
    void checkMaxLength(vector<QString> &com,unsigned long len);
    void checkMinMaxLength(vector<QString> &com,unsigned long min,unsigned long max);

    void checkAllVarType(vector<QString> &arg_vec, VariableType arg_typ);
    void checkVarType(Variable* arg_var, VariableType arg_typ);
    void checkIfVarNameFree(QString arg_name);


    VariableSet *varSet;
    Task task;

    Variable *defineVar;
    vector<QString> operation;
    Variable *result;
    SubOperationList subOpList;
};

#endif // OPERATION_H
