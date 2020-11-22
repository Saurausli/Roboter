#include <iostream>
#include <command.h>

#define ARDUINOVERSION




using namespace std;


int main()
{
    Command com;
    while(true){
        string command;

        std::getline (std::cin,command);
        cout<<command<<endl;
        com.checkCommand(command);
    }
    return 0;
}
