#pragma once

#include "Logger.hpp"

#include <string>
#include <fstream>
#include <sstream>

namespace Engine
{
    class FileManager
    {
        public:
            FileManager() = delete;
            static std::string PadString(const std::string& string, size_t num, char paddingChar = ' ');
            static std::string FileToString(const std::string& filepath);
    };
}