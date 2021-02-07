

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
    if(checkProgramm(programm)){
        connectProgramm(false);
        startProgramm();
    }

}
void ConsoleUi::loopCommand(QString programm){
    if(checkProgramm(programm)){
        connectProgramm(true);
        startProgramm();
    }
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

QVector<QString> ConsoleUi::getFuncitionKeyWords(){
   QVector<QString> vec;
   vec.push_back(TURN_SYNTAX_NAME);
   vec.push_back(SET_SYNTAX_NAME);
   vec.push_back(TEMPO_SYNTAX);
   vec.push_back(PAUSE_SYNTAX);
   vec.push_back(LABLE_SYNTAX);
   vec.push_back(GOTO_SYNTAX);
   vec.push_back(LOOPEND_SYNTAX);
   vec.push_back(LOOPSTART_SYNTAX);
   return vec;
}

QVector<QString> ConsoleUi::getVariableTypWords(){
    QVector<QString> vec;
    vec.push_back(DEF_DOUBLEMOTOR_SYNTAX);
    vec.push_back(DEF_JOINT_SYNTAX);
    return vec;
}
