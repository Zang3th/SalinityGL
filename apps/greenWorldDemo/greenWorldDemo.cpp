#include "include/greenWorldApp.hpp"

INITIALIZE_EASYLOGGINGPP

int main()
{
    //Add colorful terminal logging
    el::Loggers::addFlag(el::LoggingFlag::ColoredTerminalOutput);

    //Create app
    GreenWorldApp greenWorldApp;

    //Start app
    while(greenWorldApp.IsRunning())
        greenWorldApp.Update();

    return 0;    
}