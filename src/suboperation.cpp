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
