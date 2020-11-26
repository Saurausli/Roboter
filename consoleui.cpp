#include "consoleui.h"

ConsoleUi::ConsoleUi(QObject *parent) :
    QObject(parent){
    qDebug()<<"start";
    engine = new QQmlApplicationEngine(this);
    context = engine->rootContext();
    context->setContextProperty ("Backend", this);
    engine->load(QUrl(QStringLiteral("qrc:/main.qml")));
    //qmlRegisterType<dummy>("Dummy", 1, 0, "Ui");
    run=false;
    com =new Command;
    connect(com,SIGNAL(newRespond(QString)),this,SLOT(printRespond(QString)));
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
void ConsoleUi::printRespond(QString output){
    emit newRespond(output);
}

void ConsoleUi::runCommand(QString command){
    com->checkCommand(command.toStdString());
}
