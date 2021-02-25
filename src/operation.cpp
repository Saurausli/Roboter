#include "operation.h"

Operation::Operation(VariableSet *arg_varSet,vector<QString> arg_operation)//for Arithmetic
{
    qDebug()<<"constructor";
    result=new Variable(subOpList[subOpList.size()-1]->getResult()->getVariableType(),"res");

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
switch (task) {
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
    }
}


Variable* Operation::getResult(){
    return result;
}

void Operation::setResultVariable(Variable *arg_var){
    result=arg_var;
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
    bool operatorPossible=false;
    bool varExpected=true;
    qDebug()<<arg_operation;
    for(unsigned int i=0;i<arg_operation.size();i++){
        if(arg_operation[i]==subOperationBeginSyntax){
            bool found=false;
            unsigned int foundIndex=i+1;
            int openBrackets=1;
            qDebug()<<"search ')'";
            while(!found){
                qDebug()<<foundIndex<<" "<<arg_operation[foundIndex]<<" "<<openBrackets;
                if(foundIndex>=arg_operation.size()){
                    throw(new Error("expected ')'"));
                }
                else if(arg_operation[foundIndex]==subOperationBeginSyntax){
                    openBrackets++;
                }
                else if(arg_operation[foundIndex]==subOperationEndSyntax){
                    openBrackets--;
                }
                if(openBrackets==0){
                    if(i+1==foundIndex){
                        throw(new Error("expected expression"));
                    }
                    qDebug()<<"found ) "<<i<<" "<<foundIndex<<" "<<arg_operation[i]<< " "<<arg_operation[foundIndex];
                    found=true;
                    subOp.push_back(new Operation(varSet,vector<QString>(&arg_operation[i],&arg_operation[foundIndex])));
                    i=foundIndex+1;
                }
                else{
                    foundIndex++;
                }
            }
        }
        /*else if(varExpected){
            SubOperation::checkVarExist(varSet,arg_operation[i]);
        }
        else if(arg_operation[i]==DEF_COMMENT_SYNTAX){
            break;
        }*/
    }
    /*vector<QString> provVariable;
    arg_operation.insert(arg_operation.begin(),subOperationBeginSyntax);
    arg_operation.push_back(subOperationEndSyntax);
    do{
        unsigned lastOpenBracket=arg_operation.size();

        bool found=false;

        while(!found){
            //qDebug()<<"2";
            if(lastOpenBracket==0){
                throw(new Error("expected '('"));
            }
            lastOpenBracket--;
            if(arg_operation[lastOpenBracket]==subOperationBeginSyntax){
                found=true;
            }
        }
        found=false;
        unsigned closeBracket=lastOpenBracket;
        while(!found){
            closeBracket++;
            if(closeBracket>=arg_operation.size()){
                throw(new Error("expected ')'"));
            }
            if(arg_operation[closeBracket]==subOperationEndSyntax){
                found=true;
            }
        }
        subOpList.push_back(new SubOperation(varSet,vector<QString>(&arg_operation[lastOpenBracket],&arg_operation[closeBracket+1])));
        arg_operation.erase(arg_operation.begin()+int(lastOpenBracket),arg_operation.begin()+int(closeBracket+1));
        arg_operation.shrink_to_fit();
        arg_operation.insert(arg_operation.begin()+int(lastOpenBracket),subOpList[subOpList.size()-1]->getResult()->getName());
        }while(arg_operation.size()>1);

    task=Task::calculet;*/
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
        if((*varSet)[unsigned(SubOperation::getVariableId(varSet,arg_vec[i]))]->getVariableType()!=arg_typ){
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



