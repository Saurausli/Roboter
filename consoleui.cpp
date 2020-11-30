#include "consoleui.h"

ConsoleUi::ConsoleUi(QObject *parent) :
    Programm(parent){
    engine = new QQmlApplicationEngine(this);
    context = engine->rootContext();
    context->setContextProperty ("Backend", this);
    engine->load(QUrl(QStringLiteral("qrc:/main.qml")));
    loop=false;
}

ConsoleUi::~ConsoleUi(){

}

void ConsoleUi::tryCommand(QString programm){
    checkProgramm(programm);
    emit newRunningProgramm(programm);
}
void ConsoleUi::loopCommand(QString programm){
    loop=true;
    checkProgramm(programm);
    emit newRunningProgramm(programm);
}

void ConsoleUi::stopLoop(){
    loop=false;
}

QVector<int> ConsoleUi::getErrorLineVec(){
   QVector<int> vec;
   for(int i=0;i<int(errorList.size());i++){
        vec.push_back(errorList[i].getLine());
   }
   return vec;
}

QVector<QString> ConsoleUi::getErrorMessageVec(){
   QVector<QString> vec;
   for(int i=0;i<int(errorList.size());i++){
        vec.push_back(errorList[i].getMessage());
   }
   return vec;
}
