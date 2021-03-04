#ifndef OPERATION_H
#define OPERATION_H

#include <QObject>

#include <vector>
#include <iostream>
#include "variable.h"
#define OperatorSyntaxPlus "+"
#define OperatorSyntaxMinus "-"
#define OperatorSyntaxMultiply  "*"
#define OperatorSyntaxDivide "/"


#define OperatorSyntaxAND "&&"
#define OperatorSyntaxOR "||"
#define OperatorSyntaxGreater ">"
#define OperatorSyntaxLess "<"
#define OperatorSyntaxGreaterOrEqual ">="
#define OperatorSyntaxGreaterOrLess "<="
#define OperatorSyntaxNotEqual "!="
#define OperatorSyntaxDoubleEqual "=="

#define SignSyntaxNot "!"
#define SignSyntaxMinus "-"

#define OperatorSyntaxEqual "="

#define OperationBeginSyntax "("
#define OperationEndSyntax ")"


#define Bracket_Opening "{"
#define Bracket_Closing "}"

#define DEF_COMMENT_SYNTAX "//"

struct subOperationStruct{
    int start=-1;
    int end=-1;
    vector<QString> op;
};

enum Operator{
    none,
    plus,
    minus,
    multiply,
    divide,
    andBin,
    orBin,
    greater,
    less,
    greaterOrEqual,
    greaterOrLess,
    notEqual,
    doubleEqual,
};
enum Sign{
    negativ,
    positiv,
    neutral,
    notSign,
};

enum Task{
    defineVariable,
    calculet
};

class Operation;

typedef std::vector<Operation*> OperationList;
typedef std::vector<Sign> SignList;

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

    static vector<QString> getOperatorSyntax();
    static vector<QString> getSignSyntax();
    static Operator getOperator(QString arg_operatorName);
    static Sign getSign(QString arg_signName);
    static int getVariableId(VariableSet *arg_varSet, QString arg_name);
    static bool isOperator(QString &arg_name);
    static bool isSign(QString &arg_name);
    static bool isNumber(QString &arg_string);
    static bool isBool(QString &arg_string);

    static bool isVariable(VariableSet *arg_varSet,QString arg_name);
    static Variable* getVariable(VariableSet *arg_varSet,QString arg_name);
    static void checkVarExist(VariableSet *arg_varSet,QString arg_name);
    static void checkVarExist(VariableSet *arg_varSet,QString arg_name,int &arg_pos);
    static void checkIfVarNameFree(VariableSet *arg_varSet,QString arg_name);

    static void checklength(vector<QString> &arg_com,unsigned long arg_len);
    static void checkMinlength(vector<QString> &arg_com,unsigned long arg_len);
    static void checkMaxlength(vector<QString> &arg_com,unsigned long arg_len);
    static void checkMinMaxlength(vector<QString> &arg_com,unsigned long arg_min,unsigned long arg_max);

    static void checkAllVarType(VariableSet *arg_varSet, VariableType arg_typ);
    static void checkVarType(Variable* arg_var, VariableType arg_typ);
    static void checkVarTypFromOp(Sign arg_Sign,Variable *arg_var,Operator arg_op);
    static void calc(Variable &arg_var1,Variable &arg_var2,Operator arg_op,Variable &arg_result);
    static void copyVariable(Variable* arg_source, Variable* arg_destination);

private:
    //void arithmicSetup(vector<Variable*> arg_VarVec,Variable* arg_result ,Operator arg_Op);
    void setupDefine(VariableSet *arg_varSet,QString arg_name, VariableType arg_type,QString arg_startValue);
    void setupCalc(VariableSet *arg_varSet,vector<QString> arg_operation);



    vector<Variable*> usedVariable;
    OperationList subOp;
    SignList signList;
    vector<Operator> usedOperator;
    VariableSet *varSet;

    Variable *result;
};

#endif // OPERATION_H
