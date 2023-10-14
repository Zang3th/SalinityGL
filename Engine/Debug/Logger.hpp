#pragma once

#include <string>

namespace Engine
{
    class Logger
    {
        public:
            Logger() = delete;
            static void Init();
            static void Info(const std::string& action, const std::string& obj, const std::string& params = "");
            static void Warn(const std::string& action, const std::string& obj, const std::string& params = "");
            static void Error(const std::string& action, const std::string& obj, const std::string& params = "");
            static void Trace(const std::string& action, const std::string& obj, const std::string& params = "");
            static void Print(const std::string& msg);
            static void LineBreak();
    };
}