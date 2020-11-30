#ifndef COMMAND_H
#define COMMAND_H
#include <QObject>
class Command:{
    Q_OBJECT
    public:
    Command(int _programmLine,QString _command);

    private:
        int programmLine;
        QString command;
};



#endif
