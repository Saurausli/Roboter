#include "operation.h"

Operation::Operation(VariableSet *arg_varSet,vector<QString> arg_operation)//for Arithmetic
{
    //result=new Variable(subOpList[subOpList.size()-1]->getResult()->getVariableType(),"res");
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
void Operation::exec(){
    for(unsigned int i=0;i<subOp.size();i++){
        subOp[i]->exec();
    }
    result->setType(usedVariable[0]->getVariableType());
    result->setValue(usedVariable[0]->getValue());


    for(unsigned int i=1;i<usedVariable.size();i++){
        calc(*result,*usedVariable[i],usedOperator[i-1],*result);
    }

/*switch (task) {
    case Task::defineVariable:
        defineVar->define();
        break;
    case Task::calculet:
        for(unsigned i=0;i<subOpList.size();i++){
            subOpList[i]->exec();
        }
        if(subOpList.size()>0){
            result->setValue(subOpList[subOpList.size()-1]->getResult()->getValuetoInt());
        }
        break;
    }*/
}


Variable* Operation::getResult(){
    return result;
}

void Operation::setResultVariable(Variable *arg_var){
    result=arg_var;
}


void Operation::setupDefine(VariableSet *arg_varSet,QString arg_name, VariableType arg_type,QString arg_startValue){
    /*varSet=arg_varSet;
    checkIfVarNameFree(arg_name);
    defineVar=new Variable(arg_type,arg_name,arg_startValue);
    varSet->push_back(defineVar);
    task=Task::defineVariable;*/
}


vector<QString> Operation::getOperatorSyntax(){
    vector<QString> opVec;
    opVec.push_back(OperatorSyntaxPlus);
    opVec.push_back(OperatorSyntaxMinus);
    opVec.push_back(OperatorSyntaxEqual);
    opVec.push_back(OperatorSyntaxMultiply);
    opVec.push_back(OperatorSyntaxDivide);
    opVec.push_back(OperationEndSyntax);
    opVec.push_back(OperationBeginSyntax);
    opVec.push_back(OperatorSyntaxAND);
    opVec.push_back(OperatorSyntaxOR);
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
    if(arg_operatorName==OperatorSyntaxMultiply){
        return Operator::multiply;
    }
    if(arg_operatorName==OperatorSyntaxDivide){
        return Operator::divide;
    }
    if(arg_operatorName==OperatorSyntaxAND){
        return Operator::andBin;
    }
    if(arg_operatorName==OperatorSyntaxOR){
        return Operator::orBin;
    }
    return Operator::none;
}

bool Operation::isNumber(QString &arg_string){
    bool ok;
    arg_string.toInt(&ok,10);
    return ok;
}

bool Operation::isBool(QString &arg_string){
    return (arg_string==BoolValueTrue||arg_string==BoolValueFalse);
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

void Operation::checkVarTypFromOp(Variable *arg_var,Operator arg_op){
    if((arg_op==Operator::plus||arg_op==Operator::minus||arg_op==Operator::multiply||arg_op==Operator::divide)&&arg_var->getVariableType()==VariableType::Integer){
        return;
    }
    if((arg_op==Operator::andBin||arg_op==Operator::orBin)&&arg_var->getVariableType()==VariableType::Boolean){
        return;
    }
    throw(new Error("incompatible operator"));
}

void Operation::setupCalc(VariableSet *arg_varSet,vector<QString> arg_operation){
    varSet=arg_varSet;
    bool operatorPossible=false;
    bool varExpected=true;
    result = new Variable(VariableType::unknown,"res");
    for(unsigned int i=0;i<arg_operation.size();i++){
        if(varExpected){
            if(arg_operation[i]==OperationBeginSyntax){
                bool found=false;
                unsigned int foundIndex=i+1;
                int openBrackets=1;
                while(!found){
                    if(foundIndex>=arg_operation.size()){
                        throw(new Error("expected ')'"));
                    }
                    else if(arg_operation[foundIndex]==OperationBeginSyntax){
                        openBrackets++;
                    }
                    else if(arg_operation[foundIndex]==OperationEndSyntax){
                        openBrackets--;
                    }
                    if(openBrackets==0){
                        if(i+1==foundIndex){
                            throw(new Error("expected expression 1"));
                        }
                        found=true;
                        subOp.push_back(new Operation(varSet,vector<QString>(&arg_operation[i+1],&arg_operation[foundIndex])));
                        usedVariable.push_back(subOp[subOp.size()-1]->getResult());
                        i=foundIndex;
                    }
                    else{
                        foundIndex++;
                    }
                }
            }
            else if(isVariable(varSet,arg_operation[i])){
                checkVarExist(varSet,arg_operation[i]);
                usedVariable.push_back(getVariable(varSet,arg_operation[i]));
            }

            else{
                throw(new Error("expected expression 2"));
            }
            varExpected=false;
            operatorPossible=true;
        }

        else if(arg_operation[i]==DEF_COMMENT_SYNTAX){ // found Comment
            break;
        }

        else if(operatorPossible&&isOperator(arg_operation[i])){ // found Operator
            usedOperator.push_back(getOperator(arg_operation[i]));
            varExpected=true;
            operatorPossible=false;
        }
        else{ // found neither Variable nor Operator nor Comment
            throw(new Error("expected expression 3 "));
        }
    }
    if(varExpected){
        throw(new Error("expected expression 4"));
    }
}

void Operation::checkarg_length(vector<QString> &arg_com,unsigned long arg_len){
    checkMinarg_length(arg_com,arg_len);
    checkMaxarg_length(arg_com,arg_len);
}

void Operation::checkMinarg_length(vector<QString> &arg_com,unsigned long arg_len){
    if(arg_com.size()<arg_len){
        throw(new Error("to few arguments: found: "+QString::number(arg_com.size())+" minimum: "+QString::number(arg_len)));
    }
}
void Operation::checkMaxarg_length(vector<QString> &arg_com,unsigned long arg_len){
    if(arg_com.size()>arg_len){
        throw(new Error("to many arguments: found: "+QString::number(arg_com.size())+" maximum: "+QString::number(arg_len)));
    }
}
void Operation::checkMinMaxarg_length(vector<QString> &arg_com,unsigned long arg_min,unsigned long arg_max){
    checkMinarg_length(arg_com,arg_min);
    checkMaxarg_length(arg_com,arg_max);
}

void Operation::checkAllVarType(VariableSet *arg_varSet, VariableType arg_typ){
    for(unsigned int i=0;i<arg_varSet->size();i++){
        if((*arg_varSet)[i]->getVariableType()!=arg_typ){
            throw(new Error("not all Argument have the same type"));
        }
    }
}

void Operation::checkVarType(Variable* arg_var, VariableType arg_typ){
        if(arg_var->getVariableType()!=arg_typ){
            throw(new Error("wrong Argument"));
        }
}

void Operation::checkIfVarNameFree(VariableSet *arg_varSet,QString arg_name){
    for(unsigned long i=0;i<arg_varSet->size();i++){
        if((*arg_varSet)[i]->getName()==arg_name){
            throw(new Error(arg_name+" already exist"));
        }
    }
}

void Operation::calc(Variable &arg_var1,Variable &arg_var2,Operator arg_op,Variable &arg_result){
    VariableSet varTempSet;
    varTempSet.push_back(&arg_var1);
    varTempSet.push_back(&arg_var2);
    switch (arg_op) {
        case Operator::plus:
            checkAllVarType(&varTempSet,VariableType::Integer);
            arg_result.setType(VariableType::Integer);
            arg_result.setValue(arg_var1.getValuetoInt()+arg_var2.getValuetoInt());
            break;
        case Operator::minus:
            checkAllVarType(&varTempSet,VariableType::Integer);
            arg_result.setType(VariableType::Integer);
            arg_result.setValue(arg_var1.getValuetoInt()-arg_var2.getValuetoInt());
            break;
        case Operator::multiply:
            checkAllVarType(&varTempSet,VariableType::Integer);
            arg_result.setType(VariableType::Integer);
            arg_result.setValue(arg_var1.getValuetoInt()*arg_var2.getValuetoInt());
            break;
        case Operator::divide:
            checkAllVarType(&varTempSet,VariableType::Integer);
            arg_result.setType(VariableType::Integer);
            arg_result.setValue(arg_var1.getValuetoInt()/arg_var2.getValuetoInt());
            break;
        case Operator::andBin:
            checkAllVarType(&varTempSet,VariableType::Boolean);
            arg_result.setType(VariableType::Boolean);
            arg_result.setValue(arg_var1.getValuetoBool()&&arg_var2.getValuetoBool());
            break;
        case Operator::orBin:
            checkAllVarType(&varTempSet,VariableType::Boolean);
            arg_result.setType(VariableType::Boolean);
            arg_result.setValue(arg_var1.getValuetoBool()||arg_var2.getValuetoBool());
            break;
        default:
            break;
        }
}

void Operation::copyVariable(Variable* arg_source, Variable* arg_destination){

    if(arg_source->getVariableType()!=arg_destination->getVariableType()){
        throw(new Error("icompatible type"));
    }
    arg_destination->setValue(arg_source->getValue());
}



