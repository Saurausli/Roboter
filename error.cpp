#include "error.h"

Error::Error(unsigned _line,QString _message,QString _command)
{
    message=_message+"; ";
}

unsigned Error::getLine(){
    return line;
}

void Error::addToMessage(QString _message){
    message=_message+": "+message;
}
QString Error::getMessage(){
    return "Line "+QString::number(line+1)+" Error: "+message+" "+command;
}
