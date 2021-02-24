

#include "consoleui.h"

ConsoleUi::ConsoleUi(QObject *parent) :
    Programm(parent){
    engine = new QQmlApplicationEngine(this);
    context = engine->rootContext();
    context->setContextProperty ("Backend", this);
    engine->load(QUrl(QStringLiteral("qrc:/main.qml")));
    //loop=false;

}

ConsoleUi::~ConsoleUi(){

}

void ConsoleUi::tryProgram(QString arg_program){
    compileProgram(arg_program);
}

QString ConsoleUi::loadFile(){
    std::ifstream savefile("savefile.txt");
    stringstream buffer;
    buffer<<savefile.rdbuf();
    return QString::fromStdString(buffer.str());
}

QVector<int> ConsoleUi::getErrorLineVec(){
   QVector<int> vec;
   for(unsigned i=0;i<errorList->size();i++){
        vec.push_back((*errorList)[i]->getLine());
   }
   return vec;
}

QVector<QString> ConsoleUi::getErrorMessageVec(){
   QVector<QString> vec;
   for(unsigned i=0;i<errorList->size();i++){
        vec.push_back((*errorList)[i]->getMessage());
   }
   return vec;
}
QVector<QString> ConsoleUi::getVariableTypWords(){
    return QVector<QString>::fromStdVector(Variable::getVariableTypeSyntax());
}

QString ConsoleUi::colorIn(QString arg_line){
    arg_line=" "+arg_line+" ";
    vector<QString> keywords=getVariableTypWords();
    colorKeyWord(arg_line,keywords,"#d69aa7")
}

void ConsoleUi::colorKeyWord(QString& arg_line,QVector<QString>& arg_keywords,QString& arg_color){

}
/*
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
}*/
