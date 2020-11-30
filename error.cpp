#include "error.h"

Error::Error(unsigned _line,QString _message,QString _command)
{
    line=_line;
    message=_message+"; ";
}

unsigned Error::getLine(){
    return line;
}

void Error::addToMessage(QString _message){
    message=_message+": "+message;
}
QString Error::getMessage(){
    return "Line "+QString::number(line)+" Error: "+message+" "+command;
}


ErrorList::ErrorList(){

}

void ErrorList::newError(unsigned _line,QString _message,QString _command){
    errorVec.push_back(Error(_line,_message,_command));
}

void ErrorList::addToError(unsigned line,QString message){
    int index;
    index=findError(line);
    if(index>=0){
        errorVec[index].addToMessage(message);
    }
}

QVector<int> ErrorList::getErrorLineVec(){
   QVector<int> vec;
   for(int i=0;i<int(errorVec.size());i++){
        vec.push_back(errorVec[i].getLine());
   }
   return vec;
}

QVector<QString> ErrorList::getErrorMessageVec(){
   QVector<QString> vec;
   for(int i=0;i<int(errorVec.size());i++){
        vec.push_back(errorVec[i].getMessage());
   }
   return vec;
}

int ErrorList::size(){
    return errorVec.size();
}

int ErrorList::findError(unsigned line){
    for(int i=0;i<int(errorVec.size());i++){
        if(errorVec[i].getLine()==line){
            return i;
        }
    }
    return -1;
}
