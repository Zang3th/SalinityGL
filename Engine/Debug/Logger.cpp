#include "Logger.hpp"

#include "easylogging++.hpp"
#include "FileManager.hpp"

namespace Engine
{
    void Logger::Init()
    {
        //Add colorful terminal logging
        el::Loggers::addFlag(el::LoggingFlag::ColoredTerminalOutput);

        //Configure logger
        el::Configurations config;
        config.setToDefault();
        config.setGlobally(el::ConfigurationType::Format, "%datetime{%s:%g} [%level] %msg");
        el::Loggers::reconfigureLogger("default", config);
    }

    void Logger::Info(const std::string& action, const std::string& obj, const std::string& params)
    {
        LOG(INFO) << " | " << FileManager::PadString(action, 8)
                  << " | " << FileManager::PadString(obj, 12)
                  << " | " << params;
    }

    void Logger::Warn(const std::string& action, const std::string&obj, const std::string& params)
    {
        LOG(WARNING) << " | " << FileManager::PadString(action, 8)
                     << " | " << FileManager::PadString(obj, 12)
                     << " | " << params;
    }

    void Logger::Error(const std::string& action, const std::string& obj, const std::string& params)
    {
        LOG(ERROR) << "| "  << FileManager::PadString(action, 8)
                   << " | " << FileManager::PadString(obj, 12)
                   << " | " << params;
    }

    void Logger::Trace(const std::string& action, const std::string& obj, const std::string& params)
    {
        LOG(TRACE) << "| "  << FileManager::PadString(action, 8)
                   << " | " << FileManager::PadString(obj, 12)
                   << " | " << params;
    }

    void Logger::Print(const std::string& msg)
    {
        LOG(INFO) << " | " << msg;
    }

    void Logger::LineBreak()
    {
        std::cout << "\n";
    }
}
