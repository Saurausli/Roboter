#include "consoleui.h"

ConsoleUi::ConsoleUi(QObject *parent) :
    Command(parent){
    engine = new QQmlApplicationEngine(this);
    context = engine->rootContext();
    context->setContextProperty ("Backend", this);
    engine->load(QUrl(QStringLiteral("qrc:/main.qml")));
    loop=false;
}

ConsoleUi::~ConsoleUi(){

}

void ConsoleUi::tryCommand(QString command){
    checkCommand(command);
    emit newRunningProgramm(command);
}
void ConsoleUi::loopCommand(QString command){
    loop=true;
    checkCommand(command);
    emit newRunningProgramm(command);
}

void ConsoleUi::stopLoop(){
    loop=false;
}
