#include "variable.h"

Variable::Variable(VariableType arg_type,QString arg_name,QString arg_value){
    varSetup(arg_type,arg_name);
    setValue(arg_value);
}

Variable::Variable(VariableType arg_type,QString arg_name)
{
    varSetup(arg_type,arg_name);
    setValue(0);
}

void Variable::setValue(QString arg_value){
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
}

void Variable::setValue(int arg_value){
    value=QString::number(arg_value);
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

void Variable::getVariableTypeFromString(QString arg_name, VariableType &var_Type,bool errorMessage){
    if(arg_name==VariableSyntaxInteger){
        var_Type =VariableType::Integer;
        return;
    }
    if(arg_name==VariableSyntaxBool){
        var_Type =VariableType::Boolean;
        return;
    }
    var_Type =VariableType::unknown;
    if(errorMessage){
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
        throw(new Error("bool value wrong"));
    }
}

void Variable::varSetup(VariableType arg_type,QString arg_name){
    if(arg_name.size()>150){
       throw(new Error("Variable name to long"));
    }
    type=arg_type;
    name=arg_name;
}
