#include "suboperation.h"

SubOperation::SubOperation(VariableSet *arg_varSet,vector<QString> arg_subOperation)//for Arithmetic
{
    unsigned id=0;
    while(isVariable(arg_varSet,"#res_"+QString::number(id))){
        id++;
    }
    result=new Variable(VariableType::Integer,"#res_"+QString::number(id));
    arg_varSet->push_back(result);
    setupCalc(arg_varSet,arg_subOperation);
}
SubOperation::SubOperation(VariableSet *arg_varSet,vector<QString> arg_subOperation, Variable *res){
    result=res;
    setupCalc(arg_varSet,arg_subOperation);
}


void SubOperation::exec(){
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
                    case Operator::multiply:
                        result->setValue(result->getValuetoInt()*getVariable(varSet,operation[i])->getValuetoInt());
                    break;
                    case Operator::divide:
                        result->setValue(result->getValuetoInt()/getVariable(varSet,operation[i])->getValuetoInt());
                    break;
                    case Operator::none:
                        throw(new Error("error Operator unknown"));
                }
            }
        }
}

Variable* SubOperation::getResult(){
    return result;
}

vector<QString> SubOperation::getOperatorSyntax(){
    vector<QString> opVec;
    opVec.push_back(OperatorSyntaxPlus);
    opVec.push_back(OperatorSyntaxMinus);
    opVec.push_back(OperatorSyntaxEqual);
    opVec.push_back(OperatorSyntaxMultiply);
    opVec.push_back(OperatorSyntaxDivide);
    opVec.push_back(subOperationEndSyntax);
    opVec.push_back(subOperationBeginSyntax);
    opVec.push_back(OperatorSyntaxAND);
    opVec.push_back(OperatorSyntaxOR);
    return opVec;
}

bool SubOperation::isOperator(QString &arg_name){
    vector<QString> op;
    op=getOperatorSyntax();
    for(unsigned int i=0;i<op.size();i++){
        if(op[i]==arg_name){
            return true;
        }
    }
    return false;
}

int SubOperation::getVariableId(VariableSet *arg_varSet, QString arg_name){
    for(unsigned int i=0;i<arg_varSet->size();i++){
        if((*arg_varSet)[i]->getName()==arg_name){
            return int(i);
        }
    }
    return -1;
}

Operator SubOperation::getOperator(QString arg_operatorName){
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

bool SubOperation::isNumber(QString &arg_string){
    bool ok;
    arg_string.toInt(&ok,10);
    return ok;
}

bool SubOperation::isVariable(VariableSet *arg_varSet,QString arg_name){
    for(unsigned long i=0;i<arg_varSet->size();i++){
        if((*arg_varSet)[i]->getName()==arg_name){
            return true;
        }
    }
    return false;
}

Variable* SubOperation::getVariable(VariableSet *arg_varSet,QString arg_name){
    int pos;
    checkVarExist(arg_varSet,arg_name,pos);
    return (*arg_varSet)[unsigned(pos)];
}
void SubOperation::checkVarExist(VariableSet *arg_varSet,QString arg_name){
    int x;
    checkVarExist(arg_varSet,arg_name,x);
}
void SubOperation::checkVarExist(VariableSet *arg_varSet,QString arg_name,int &arg_pos){
    for(unsigned long i=0;i<arg_varSet->size();i++){
        if((*arg_varSet)[i]->getName()==arg_name){
            arg_pos=int(i);
            return;
        }
    }
    throw(new Error(arg_name+" not defined"));
}

void SubOperation::checkVarTypFromOp(Variable *arg_var,Operator arg_op){
    if((arg_op==Operator::plus||arg_op==Operator::minus||arg_op==Operator::multiply||arg_op==Operator::divide)&&arg_var->getVariableType()==VariableType::Integer){
        return;
    }
    if((arg_op==Operator::andBin||arg_op==Operator::orBin)&&arg_var->getVariableType()==VariableType::Boolean){
        return;
    }
    throw(new Error("incompatible operator"));
    return;
}

void SubOperation::setupCalc(VariableSet *arg_varSet,vector<QString> arg_operation){

    arg_operation.erase(remove(arg_operation.begin(),arg_operation.end(),subOperationEndSyntax),arg_operation.end());
    arg_operation.erase(remove(arg_operation.begin(),arg_operation.end(),subOperationBeginSyntax),arg_operation.end());
    arg_operation.shrink_to_fit();
    //qDebug()<<"after shrink "<<arg_operation;
    varSet=arg_varSet;
    operation=arg_operation;
    vector<QString> provVariable;

    if(arg_operation.size()>0){
        SubOperation::checkVarExist(varSet,arg_operation[0]);
        bool val=false;

        for (unsigned int i=1;i<arg_operation.size();i++) {
            if(val&&!SubOperation::isOperator(arg_operation[i])){
                SubOperation::checkVarExist(varSet,arg_operation[i]);
                SubOperation::checkVarTypFromOp(getVariable(varSet,arg_operation[i]),getOperator(arg_operation[i-1]));
                val=false;
            }
            else if(!val&&isOperator(arg_operation[i])){
                SubOperation::checkVarTypFromOp(getVariable(varSet,arg_operation[i-1]),getOperator(arg_operation[i]));
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
}
