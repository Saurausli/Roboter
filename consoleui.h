#ifndef CONSOLEUI_H
#define CONSOLEUI_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <vector>
#include "command.h"

class ConsoleUi: public QObject
{
    Q_OBJECT
public:
    explicit ConsoleUi(QObject *parent= nullptr);
    ~ConsoleUi();

signals:
    void newRespond(QString respond);
public slots:
    void tryCommand(QString command);
    void loopCommand(QString command);
    void stop();
    void printRespond(QString output);
private:
    QQmlApplicationEngine *engine;
    QQmlContext* context;
    Command* com;
    bool run;
    void runCommand(QString command);
};

#endif // CONSOLEUI_H
