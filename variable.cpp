#include "variable.h"

Variable::Variable(VariableType arg_type,QString arg_name){
    type=arg_type;
    name=arg_name;
}

Variable::Variable(VariableType arg_type)
{
    Variable(arg_type,"");
}

void Variable::setValue(QString arg_value){

    try {
        switch (type) {
            case integer:
                checkNumber(arg_value);
                arg_value.toInt();
        }
    }
    catch(Error *er){
        qDebug()<<er->getMessage();
    }
}

int Variable::getValuetoInt(){
    return value->toInt();
}

QString Variable::getName(){

}
VariableType Variable::getVariableType(){

}
void Variable::checkNumber(QString number){
    bool ok;
    number.toInt(&ok,10);
    if(!ok){
        throw(new Error(number+" isn't a number"));
    }
}