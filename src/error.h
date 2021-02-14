#ifndef ERROR_H
#define ERROR_H

#include <QObject>

using namespace std;

class Error;

typedef std::vector<Error*> ErrorList;

class Error
{
public:
    Error(QString arg_message);
    void setLine(unsigned arg_line);
    int getLine();
    void addToMessage(QString arg_message);
    QString getMessage();

private:
    int line;
    QString message;
    QString command;
};
#endif // ERROR_H
