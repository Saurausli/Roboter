#include "operation.h"

Operation::Operation(vector<Variable*> arg_VarVec,Operator arg_Op)
{
    varVec=arg_VarVec;
    operatorSymbol=arg_Op;
    try{
        switch (operatorSymbol) {
                case Operator::plus:
                    checkLength(varVec,2);
                    checkAllVarType(arg_VarVec,VariableType::integer);
                    result= new Variable(VariableType::integer);
            break;
            case Operator::minus:
                    checkLength(varVec,2);
                    checkAllVarType(arg_VarVec,VariableType::integer);
                    result= new Variable(VariableType::integer);
                    break;
        }
    }
    catch(Error *er){
        qDebug()<<er->getMessage();
    }
}

Variable* Operation::getResult(){
    return result;
}
void Operation::calc(){
    switch (operatorSymbol) {
            case Operator::plus:
                result->setValue(varVec[0]->getValuetoInt()+varVec[1]->getValuetoInt());
                break;
            case Operator::minus:
                result->setValue(varVec[0]->getValuetoInt()-varVec[1]->getValuetoInt());
                break;
    }
    qDebug()<<result->getValuetoInt();
}

Operator Operation::getOperator(QString arg_operatorName){
    if(arg_operatorName==OperatorSyntaxPlus){
        return Operator::plus;
    }
    if(arg_operatorName==OperatorSyntaxMinus){
        return Operator::plus;
    }
    return Operator::none;
}

void Operation::checkLength(vector<Variable*>  &com,unsigned long len){
    checkMinLength(com,len);
    checkMaxLength(com,len);
}

void Operation::checkMinLength(vector<Variable*>  &com,unsigned long len){
    if(com.size()<len){
        throw(new Error("to few arguments: found: "+QString::number(com.size())+" minimum: "+QString::number(len)));
    }
}
void Operation::checkMaxLength(vector<Variable*>  &com,unsigned long len){
    if(com.size()>len){
        throw(new Error("to many arguments: found: "+QString::number(com.size())+" maximum: "+QString::number(len)));
    }
}
void Operation::checkMinMaxLength(vector<Variable*> &com,unsigned long min,unsigned long max){
    checkMinLength(com,min);
    checkMaxLength(com,max);
}

void Operation::checkAllVarType(vector<Variable*> arg_vec, VariableType arg_typ){
    for(unsigned int i=0;i<arg_vec.size();i++){
        if(arg_vec[i]->getVariableType()!=arg_typ){
            throw(new Error("not all Argument have the same type"));
        }
    }
}
