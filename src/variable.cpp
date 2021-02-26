#include "variable.h"

Variable::Variable(VariableType arg_type,QString arg_name,QString arg_value){
    varSetup(arg_type,arg_name);
    setValue(arg_value);
}

Variable::Variable(VariableType arg_type,QString arg_name)
{
    varSetup(arg_type,arg_name);
    switch (type) {
        case Integer:
            setValue(0);
            break;
        case Boolean:
            setValue(false);
            break;

    }
    ;
}

void Variable::setValue(QString arg_value){
    qDebug()<<"string "<<name<<" "<<arg_value;
    try {
        value=arg_value;
        switch (type) {
            case Integer:
                checkNumber(value);
                break;
            case Boolean:
                checkBool(value);
                break;
        }
    }
    catch(Error *er){
        qDebug()<<er->getMessage();
    }
    qDebug()<<value;
}

void Variable::setValue(int arg_value){
    qDebug()<<"int";
    value=QString::number(arg_value);
}

void Variable::setValue(bool arg_value){
    qDebug()<<"bool "<<name;
    if(arg_value){
        value=BoolValueTrue;
    }
    else{
        value=BoolValueFalse;
    }
    qDebug()<<arg_value;
}

void Variable::setType(VariableType arg_type){
    type=arg_type;
}

void Variable::define(){
    defined=true;
}



int Variable::getValuetoInt(){
       return value.toInt();
    /*}
    else{
        qDebug()<<name<<" not defined";
        return -1;
    }*/
}

bool Variable::getValuetoBool(){
    return toBool(value);
}

QString Variable::getValue(){
    return value;
}

QString Variable::getName(){
    return name;
}
VariableType Variable::getVariableType(){
    return type;
}

bool Variable::checkIfIsVariableType(QString arg_type){

    VariableType varType;
    getVariableTypeFromString(arg_type,varType);
    return varType!=VariableType::unknown;
}

void Variable::checkNumber(QString number){
    bool ok;
    number.toInt(&ok,10);
    if(!ok){
        throw(new Error(number+" isn't a number"));
    }
}

VariableType Variable::getVariableTypeFromString(QString arg_name){
    if(arg_name==VariableSyntaxInteger){
        return VariableType::Integer;
    }
    if(arg_name==VariableSyntaxBool){
        return VariableType::Boolean;
    }
    return VariableType::unknown;
}

void Variable::getVariableTypeFromString(QString arg_name, VariableType &var_Type,bool errorMessage){
    var_Type=getVariableTypeFromString(arg_name);
    if(errorMessage&&var_Type==VariableType::unknown){
        throw(new Error(arg_name +" is no varible type"));
    }
}

vector<QString> Variable::getVariableTypeSyntax(){
    vector<QString> ret;
    ret.push_back(VariableSyntaxInteger);
    return ret;
}

bool Variable::toBool(QString arg_value){
    if(arg_value==BoolValueTrue){
        return true;
    }
    if(arg_value==BoolValueFalse){
        return false;
    }
    return false;
}
void Variable::checkBool(QString arg_value){
    if(!(arg_value==BoolValueTrue||arg_value==BoolValueFalse)){
        throw(new Error("bool value wrong "+arg_value));
    }
}

void Variable::varSetup(VariableType arg_type,QString arg_name){
    if(arg_name.size()>150){
       throw(new Error("Variable name to long"));
    }
    type=arg_type;
    name=arg_name;
}
