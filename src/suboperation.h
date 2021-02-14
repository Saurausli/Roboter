#ifndef SUBOPERATION_H
#define SUBOPERATION_H

#include <QObject>
#include "variable.h"

#define OperatorSyntaxPlus "+"
#define OperatorSyntaxMinus "-"
#define OperatorSyntaxMultiply  "*"
#define OperatorSyntaxDivide "/"

#define OperatorSyntaxEqual "="

#define subOperationBeginSyntax "("
#define subOperationEndSyntax ")"

using namespace std;


enum Operator{
    none,
    plus,
    minus,
    multiply,
    divide
};
class SubOperation;

typedef std::vector<SubOperation*> SubOperationList;

class SubOperation
{
public:
    SubOperation(VariableSet *arg_varSet,vector<QString> arg_subOperation); //for Arithmetic
    SubOperation(VariableSet *arg_varSet,vector<QString> arg_subOperation, Variable *arg_res); //for Arithmetic

    void setResultVariable(Variable *arg_var);
    void exec();
    Variable* getResult();

    static vector<QString> getOperatorSyntax();
    static Operator getOperator(QString arg_operatorName);
    static int getVariableId(VariableSet *arg_varSet, QString arg_name);
    static bool isOperator(QString &arg_name);
    static bool isNumber(QString &arg_string);
    static bool isVariable(VariableSet *arg_varSet,QString arg_name);
    static Variable* getVariable(VariableSet *arg_varSet,QString arg_name);
    static void checkVarExist(VariableSet *arg_varSet,QString arg_name);
    static void checkVarExist(VariableSet *arg_varSet,QString arg_name,int &arg_pos);

private:
    void setupCalc(VariableSet *arg_varSet,vector<QString> arg_operation);
    Variable *result;
    VariableSet *varSet;
    vector<QString> operation;
};

#endif // SUBOPERATION_H
