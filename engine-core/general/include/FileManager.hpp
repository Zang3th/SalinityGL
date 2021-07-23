#pragma once

#include "../../vendor/include/easylogging++.hpp"
#include <string>
#include <fstream>
#include <sstream>

namespace Core
{
    class FileManager
    {
        public:
            static std::string FileToString(const std::string& filepath);
    };
}