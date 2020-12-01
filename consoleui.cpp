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
    if(checkProgramm(programm)){
        for(int i=0;i<(programmVec.size()-1);i++){
            connect(programmVec[i],SIGNAL(commandFinished()),programmVec[i+1],SLOT(exec()));
        }
        programmVec[0]->exec();
    }
    emit newRunningProgramm(programm);

}
void ConsoleUi::loopCommand(QString programm){
    loop=true;
    if(checkProgramm(programm)){
        connect(programmVec[programmVec.size()-1],SIGNAL(commandFinished()),programmVec[0],SLOT(exec()));
        connect(programmVec[programmVec.size()-1],SIGNAL(commandFinished()),this,SIGNAL(newRunningCommand()));
    }
    tryCommand(programm);
}

void ConsoleUi::stopLoop(){
    loop=false;
    disconnect(programmVec[programmVec.size()-1],SIGNAL(commandFinished()),programmVec[0],SLOT(exec()));
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

QString ConsoleUi::loadFile(){
    std::ifstream savefile("savefile.txt");
    stringstream buffer;
    buffer<<savefile.rdbuf();
    return QString::fromStdString(buffer.str());
}
