#include "include/greenWorldApp.hpp"

INITIALIZE_EASYLOGGINGPP

int main()
{
    //Add colorful terminal logging
    el::Loggers::addFlag(el::LoggingFlag::ColoredTerminalOutput);

    //Configure logger
    el::Configurations defaultConf;
    defaultConf.setToDefault();
    defaultConf.setGlobally(el::ConfigurationType::Format, "%datetime{%H:%m:%s} [%level] [%fbase] %msg");
    el::Loggers::reconfigureLogger("default", defaultConf);

    //Create app
    GreenWorldApp greenWorldApp;

    //Start app
    while(greenWorldApp.IsRunning())
        greenWorldApp.Update();

    return 0;    
}