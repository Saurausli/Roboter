#include "operation.h"

Operation::Operation(vector<Variable*> arg_VarVec,Variable* arg_result ,Operator arg_Op)
{
    arithmicSetup(arg_VarVec,arg_result ,arg_Op);
}
Operation::Operation(Variable* arg_Var,Variable* arg_result){
    vector<Variable*> arg_vecVar;
    arg_vecVar.push_back(arg_Var);
    arithmicSetup(arg_vecVar,arg_result ,Operator::overwrite);
}

Operation::Operation(Variable* arg_result ,Operator arg_Op,vector<Variable*> *arg_defineVec){
    defineVec=arg_defineVec;
    setResultVariable(arg_result);
    operatorSymbol=arg_Op;
}

Variable* Operation::getResult(){
    return result;
}

void Operation::setResultVariable(Variable *arg_var){
    result=arg_var;
}

void Operation::exec(){
    switch (operatorSymbol) {
            case Operator::plus:
                result->setValue(varVec[0]->getValuetoInt()+varVec[1]->getValuetoInt());
                break;
            case Operator::minus:
                result->setValue(varVec[0]->getValuetoInt()-varVec[1]->getValuetoInt());
                break;
            case Operator::defineVariable:
                varVec.push_back(result);
                break;
            case Operator::overwrite:
                *result=*varVec[0];
                break;

    }
    qDebug()<<result->getValuetoInt();
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

void Operation::arithmicSetup(vector<Variable*> arg_VarVec,Variable* arg_result ,Operator arg_Op){
    varVec=arg_VarVec;
    operatorSymbol=arg_Op;
    setResultVariable(arg_result);
    try{
        switch (operatorSymbol) {
                case Operator::plus:

                    checkLength(varVec,2);
                    checkVarType(result,VariableType::integer);
                    checkAllVarType(arg_VarVec,VariableType::integer);
                    result= new Variable(VariableType::integer,"res");
            break;
            case Operator::minus:
                    checkLength(varVec,2);
                    arg_VarVec.push_back(arg_result);
                    checkVarType(result,VariableType::integer);
                    checkAllVarType(arg_VarVec,VariableType::integer);
                    result= new Variable(VariableType::integer,"res");
                    break;
            case Operator::overwrite:
                    checkLength(varVec,1);
                    checkVarType(result,VariableType::integer);
                    checkAllVarType(arg_VarVec,VariableType::integer);
                    result= new Variable(VariableType::integer,"res");
        }

    }
    catch(Error *er){
        throw(er);
    }
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

void Operation::checkVarType(Variable* arg_var, VariableType arg_typ){
        if(arg_var->getVariableType()!=arg_typ){
            throw(new Error("wrong Argument"));
        }
}
