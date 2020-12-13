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

signals:
    void getStop();
public slots:
    void tryCommand(QString programm);
    void loopCommand(QString programm);
    void stopLoop();
    QVector<int> getErrorLineVec();
    QVector<QString> getErrorMessageVec();
    QString loadFile();
    QVector<QString> getFuncitionKeyWords();
private:
    QQmlApplicationEngine *engine;
    QQmlContext* context;
};

#endif // CONSOLEUI_H
