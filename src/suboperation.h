#ifndef SUBOPERATION_H
#define SUBOPERATION_H

#include <QObject>
#include "variable.h"



using namespace std;




class SubOperation;



class SubOperation
{
public:
    SubOperation(VariableSet *arg_varSet,vector<QString> arg_subOperation); //for Arithmetic
    SubOperation(VariableSet *arg_varSet,vector<QString> arg_subOperation, Variable *arg_res); //for Arithmetic

    void setResultVariable(Variable *arg_var);
    void exec();
    Variable* getResult();



private:
    void setupCalc(VariableSet *arg_varSet,vector<QString> arg_operation);
    Variable *result;
    VariableSet *varSet;
    vector<QString> operation;
};

#endif // SUBOPERATION_H
