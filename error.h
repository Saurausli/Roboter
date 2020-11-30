#ifndef ERROR_H
#define ERROR_H

#include <QObject>

using namespace std;

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

class ErrorList
{
public:
    ErrorList();
    void newError(unsigned _line,QString _message,QString _command);
    void addToError(unsigned line,QString message);
    QVector<int> getErrorLineVec();
    QVector<QString> getErrorMessageVec();
    int size();
private:
    int findError(unsigned line);
    vector<Error> errorVec;
};
#endif // ERROR_H
