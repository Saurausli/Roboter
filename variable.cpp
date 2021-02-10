#include "variable.h"

Variable::Variable(VariableType arg_type,QString arg_name){
    varSetup(arg_type,arg_name);
}

Variable::Variable(VariableType arg_type)
{
    varSetup(arg_type,"");
}

void Variable::setValue(QString arg_value){

    try {
        switch (type) {
            case integer:
                checkNumber(arg_value);
                value=new QVariant(arg_value.toInt());
        }
    }
    catch(Error *er){
        qDebug()<<er->getMessage();
    }
}

void Variable::setValue(int arg_value){
    value=new QVariant(arg_value);
}

int Variable::getValuetoInt(){
    return value->toInt();
}

QString Variable::getName(){
    return name;
}
VariableType Variable::getVariableType(){
    return type;
}
void Variable::checkNumber(QString number){
    bool ok;
    number.toInt(&ok,10);
    if(!ok){
        throw(new Error(number+" isn't a number"));
    }
}
void Variable::varSetup(VariableType arg_type,QString arg_name){
    type=arg_type;
    name=arg_name;
}
