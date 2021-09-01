#include "Logger.hpp"

namespace Core
{
    void Logger::Init()
    {
        //Add colorful terminal logging
        el::Loggers::addFlag(el::LoggingFlag::ColoredTerminalOutput);

        //Configure logger
        el::Configurations defaultConf;
        defaultConf.setToDefault();
        defaultConf.setGlobally(el::ConfigurationType::Format, "%datetime{%s:%g} [%level] %msg");
        el::Loggers::reconfigureLogger("default", defaultConf);
    }
}