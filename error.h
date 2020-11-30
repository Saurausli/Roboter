#ifndef ERROR_H
#define ERROR_H

#include <QObject>

using namespace std;

class Error;

typedef std::vector<Error> ErrorList;

class Error
{
public:
    Error(unsigned _line,QString _message,QString _command);
    unsigned getLine();
    void addToMessage(QString _message);
    QString getMessage();

private:
    int line;
    QString message;
    QString command;
};
#endif // ERROR_H
