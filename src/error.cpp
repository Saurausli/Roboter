#include "error.h"

Error::Error(QString arg_message)
{
    message=arg_message+"; ";
}

void Error::setLine(unsigned arg_line){
    line=int(arg_line);
}

int Error::getLine(){
    return line;
}

void Error::addToMessage(QString arg_message){
    message=arg_message+": "+message;
}
QString Error::getMessage(){
    return "Line "+QString::number(line+1)+" Error: "+message+" "+command;
}
