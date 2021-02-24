#ifndef CONSOLEUI_H
#define CONSOLEUI_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <vector>
#include "programm.h"

class ConsoleUi: public Programm
{
    Q_OBJECT
public:
    explicit ConsoleUi(QObject *parent= nullptr);
    ~ConsoleUi();
public slots:
    void tryProgram(QString arg_program);
    QString loadFile();
    //void loopProgram(QString arg_program);
/*
signals:
    void getStop();*/
public slots:
    QVector<int> getErrorLineVec();
    QVector<QString> getErrorMessageVec();
    QVector<QString> getVariableTypWords();
    QString colorIn(QString arg_line);
    /*void tryCommand(QString programm);
    void loopCommand(QString programm);
    void stopLoop();
    QVector<QString> getFuncitionKeyWords();
    QVector<QString> getVariableTypWords();*/
private:
    QQmlApplicationEngine *engine;
    QQmlContext* context;
    void colorKeyWord(QString& arg_line,vector<QString> arg_keywords,QString arg_color);
};

#endif // CONSOLEUI_H
