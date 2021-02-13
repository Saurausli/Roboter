#include "operation.h"

Operation::Operation(VariableSet *arg_varSet,vector<QString> arg_operation)//for Arithmetic
{
    setupCalc(arg_varSet,arg_operation);
}
Operation::Operation(VariableSet *arg_varSet,vector<QString> arg_operation, Variable *res){
    result=res;
    setupCalc(arg_varSet,arg_operation);
}

Operation::Operation(VariableSet *arg_varSet,QString arg_name, QString arg_type){
    VariableType varType;
    Variable::getVariableTypeFromString(arg_type,varType,true);
    setupDefine(arg_varSet,arg_name,varType,"0");
}
Operation::Operation(VariableSet *arg_varSet,QString arg_name, VariableType arg_type){
    setupDefine(arg_varSet,arg_name,arg_type,"0");
}

Operation::Operation(VariableSet *arg_varSet,QString arg_name, VariableType arg_type,QString arg_startValue){
    setupDefine(arg_varSet,arg_name,arg_type,arg_startValue);
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
            defineVar->define();
            break;
        case Task::calculet:
            //result = getVariable(operation[0]);
            result->setValue(getVariable(varSet,operation[0])->getValuetoInt());
            Operator op=Operator::none;
            for(unsigned int i=1;i<operation.size();i++){
                if(i%2==1){
                    op=getOperator(operation[i]);
                }
                else{
                    switch(op){
                        case Operator::plus:
                          result->setValue(result->getValuetoInt()+getVariable(varSet,operation[i])->getValuetoInt());
                        break;
                        case Operator::minus:
                          result->setValue(result->getValuetoInt()-getVariable(varSet,operation[i])->getValuetoInt());
                        break;
                        case Operator::none:
                            throw(new Error("error Oprator unknown"));
                    }
                }
            }

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

int Operation::getVariableId(VariableSet *arg_varSet, QString arg_name){
    for(unsigned int i=0;i<arg_varSet->size();i++){
        if((*arg_varSet)[i]->getName()==arg_name){
            return int(i);
        }
    }
    return -1;
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

bool Operation::isNumber(QString &arg_string){
    bool ok;
    arg_string.toInt(&ok,10);
    return ok;
}

bool Operation::isVariable(VariableSet *arg_varSet,QString arg_name){
    for(unsigned long i=0;i<arg_varSet->size();i++){
        if((*arg_varSet)[i]->getName()==arg_name){
            return true;
        }
    }
    return false;
}

Variable* Operation::getVariable(VariableSet *arg_varSet,QString arg_name){
    int pos;
    checkVarExist(arg_varSet,arg_name,pos);
    return (*arg_varSet)[unsigned(pos)];
}
void Operation::checkVarExist(VariableSet *arg_varSet,QString arg_name){
    int x;
    checkVarExist(arg_varSet,arg_name,x);
}
void Operation::checkVarExist(VariableSet *arg_varSet,QString arg_name,int &arg_pos){
    for(unsigned long i=0;i<arg_varSet->size();i++){
        if((*arg_varSet)[i]->getName()==arg_name){
            arg_pos=int(i);
            return;
        }
    }
    throw(new Error(arg_name+" not defined"));
}
void Operation::setupDefine(VariableSet *arg_varSet,QString arg_name, VariableType arg_type,QString arg_startValue){
    varSet=arg_varSet;
    checkIfVarNameFree(arg_name);
    defineVar=new Variable(arg_type,arg_name,arg_startValue);
    varSet->push_back(defineVar);
    task=Task::defineVariable;
}

void Operation::setupCalc(VariableSet *arg_varSet,vector<QString> arg_operation){
    varSet=arg_varSet;
    operation=arg_operation;
    vector<QString> provVariable;

    if(arg_operation.size()>0){
        checkVarExist(varSet,arg_operation[0]);
        bool val=false;
        for (unsigned int i=1;i<arg_operation.size();i++) {
            if(val&&!isOperator(arg_operation[i])){
                checkVarExist(varSet,arg_operation[i]);
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
    else{
        throw(new Error("expected expression"));
    }
    task=Task::calculet;
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
        if((*varSet)[unsigned(getVariableId(varSet,arg_vec[i]))]->getVariableType()!=arg_typ){
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
    for(unsigned long i=0;i<varSet->size();i++){
        if((*varSet)[i]->getName()==arg_name){
            throw(new Error(arg_name+" already exist"));
        }
    }
}



