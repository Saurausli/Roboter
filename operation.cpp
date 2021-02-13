#include "operation.h"

Operation::Operation(VariableSet *arg_varSet,vector<QString> arg_operation)//for Arithmetic
{
    varSet=arg_varSet;
    operation=arg_operation;
    vector<QString> provVariable;
    checkVarExist(arg_operation[0]);
    if(arg_operation[1]==OperatorSyntaxEqual){
        if(arg_operation.size()==2){
            throw(new Error("expected expression"));
        }
        else{
            bool val=true;
            for (unsigned int i=2;i<arg_operation.size();i++) {
                if(val&&!isOperator(arg_operation[i])){
                    checkVarExist(arg_operation[i]);
                    val=false;
                }
                else if(!val&&isOperator(arg_operation[i])){
                    val=true;
                }
                else{
                   throw(new Error("expected expression"));
                }
            }
            if(val){
                throw(new Error("expected expression"));
            }
        }
    }

    task=Task::calculet;
    //arithmicSetup(arg_VarVec,arg_result ,arg_Op);

}

Operation::Operation(VariableSet *arg_varSet,QString arg_name, QString arg_type){
    Variable::getVariableTypeFromString(arg_type,varType,true);
    setupDefine(arg_varSet,arg_name,varType);
}
Operation::Operation(VariableSet *arg_varSet,QString arg_name, VariableType arg_type){
    setupDefine(arg_varSet,arg_name,arg_type);
}

Operation::Operation(VariableSet *arg_varSet,QString arg_name, VariableType arg_type,QVariant arg_startValue){
    startValue=arg_startValue;
    setupDefine(arg_varSet,arg_name,arg_type);
}
Variable* Operation::getResult(){
    return result;
}

void Operation::setResultVariable(Variable *arg_var){
    result=arg_var;
}

void Operation::exec(){
    switch (task) {
        case Task::defineVariable:
            varSet->variable.push_back(new Variable(varType,varName,startValue));
            break;
        case Task::calculet:

            break;
    }
}

vector<QString> Operation::getOperatorSyntax(){
    vector<QString> opVec;
    opVec.push_back(OperatorSyntaxPlus);
    opVec.push_back(OperatorSyntaxMinus);
    opVec.push_back(OperatorSyntaxEqual);
    return opVec;
}

bool Operation::isOperator(QString &arg_name){
    vector<QString> op;
    op=getOperatorSyntax();
    for(unsigned int i=0;i<op.size();i++){
        if(op[i]==arg_name){
            return true;
        }
    }
    return false;
}

bool Operation::isNumber(QString &arg_string){
    bool ok;
    arg_string.toInt(&ok,10);
    return ok;
}

Operator Operation::getOperator(QString arg_operatorName){
    if(arg_operatorName==OperatorSyntaxPlus){
        return Operator::plus;
    }
    if(arg_operatorName==OperatorSyntaxMinus){
        return Operator::minus;
    }
    return Operator::none;
}

int Operation::getProvVariableId(VariableSet *arg_varSet, QString arg_name){
    for(unsigned int i=0;i<arg_varSet->names.size();i++){
        if(arg_varSet->names[i]==arg_name){
            return int(i);
        }
    }
    return -1;
}

void Operation::setupDefine(VariableSet *arg_varSet,QString arg_name, VariableType arg_type){
    varSet=arg_varSet;
    checkIfVarNameFree(arg_name);
    varName=arg_name;
    varType=arg_type;

    varSet->names.push_back(varName);
    varSet->type.push_back(varType);

    task=Task::defineVariable;
}

void Operation::checkLength(vector<QString> &com,unsigned long len){
    checkMinLength(com,len);
    checkMaxLength(com,len);
}

void Operation::checkMinLength(vector<QString> &com,unsigned long len){
    if(com.size()<len){
        throw(new Error("to few arguments: found: "+QString::number(com.size())+" minimum: "+QString::number(len)));
    }
}
void Operation::checkMaxLength(vector<QString> &com,unsigned long len){
    if(com.size()>len){
        throw(new Error("to many arguments: found: "+QString::number(com.size())+" maximum: "+QString::number(len)));
    }
}
void Operation::checkMinMaxLength(vector<QString> &com,unsigned long min,unsigned long max){
    checkMinLength(com,min);
    checkMaxLength(com,max);
}

void Operation::checkAllVarType(vector<QString> &arg_vec, VariableType arg_typ){
    for(unsigned int i=0;i<arg_vec.size();i++){
        if(varSet->type[unsigned(getProvVariableId(varSet,arg_vec[i]))]!=arg_typ){
            throw(new Error("not all Argument have the same type"));
        }
    }
}

void Operation::checkVarType(Variable* arg_var, VariableType arg_typ){
        if(arg_var->getVariableType()!=arg_typ){
            throw(new Error("wrong Argument"));
        }
}

void Operation::checkIfVarNameFree(QString arg_name){
    for(unsigned long i=0;i<varSet->names.size();i++){
        if(varSet->names[i]==arg_name){
            throw(new Error(arg_name+" already exist"));
        }
    }
}
void Operation::checkVarExist(QString arg_name){
    for(unsigned long i=0;i<varSet->names.size();i++){
        if(varSet->names[i]==arg_name){
            return;
        }
    }
    throw(new Error(arg_name+" not defined"));
}
