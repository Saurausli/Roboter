#ifndef CONSOLEUI_H
#define CONSOLEUI_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <vector>
#include "command.h"

class ConsoleUi: public Command
{
    Q_OBJECT
public:
    explicit ConsoleUi(QObject *parent= nullptr);
    ~ConsoleUi();

signals:

public slots:
    void tryCommand(QString command);
    void loopCommand(QString command);
    void stop();
private:
    QQmlApplicationEngine *engine;
    QQmlContext* context;
    bool run;
    void runCommand(QString command);
};

#endif // CONSOLEUI_H
