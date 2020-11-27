#include "consoleui.h"

ConsoleUi::ConsoleUi(QObject *parent) :
    Command(parent){
    qDebug()<<"start";
    engine = new QQmlApplicationEngine(this);
    context = engine->rootContext();
    context->setContextProperty ("Backend", this);
    engine->load(QUrl(QStringLiteral("qrc:/main.qml")));
    run=false;
}

ConsoleUi::~ConsoleUi(){

}

void ConsoleUi::tryCommand(QString command){
    runCommand(command);
}
void ConsoleUi::loopCommand(QString command){
    while(true){
        runCommand(command);
    }
}
void ConsoleUi::stop(){

}

void ConsoleUi::runCommand(QString command){
    checkCommand(command);
}
