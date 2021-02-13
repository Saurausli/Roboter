#include "variable.h"

Variable::Variable(VariableType arg_type,QString arg_name,QString arg_value){
    varSetup(arg_type,arg_name);
    setValue(arg_value);
}

Variable::Variable(VariableType arg_type,QString arg_name)
{
    varSetup(arg_type,arg_name);
}

void Variable::setValue(QString arg_value){
    try {
        value=arg_value;
        switch (type) {
            case integer:
                checkNumber(value);

        }
    }
    catch(Error *er){
        qDebug()<<er->getMessage();
    }
}

void Variable::setValue(int arg_value){
    value=QString::number(arg_value);
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
        var_Type =VariableType::integer;
        return;
    }
    var_Type =VariableType::unknown;
    if(errorMessage){
        throw(new Error(arg_name +" is no Varible Type"));
    }
}

vector<QString> Variable::getVariableTypeSyntax(){
    vector<QString> ret;
    ret.push_back(VariableSyntaxInteger);
    return ret;
}

void Variable::varSetup(VariableType arg_type,QString arg_name){
    type=arg_type;
    name=arg_name;
}
