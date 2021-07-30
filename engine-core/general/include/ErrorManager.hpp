#pragma once

#include "../../vendor/include/glad.hpp"
#include "../../vendor/include/easylogging++.hpp"
#include <csignal>

#define ASSERT(x) if(!(x)) raise(SIGTRAP);
#define GLCall(x) ErrorManager::GLClearError();\
x;\
ASSERT(ErrorManager::GLLogCall(#x, __FILE__, __LINE__))

namespace Core
{
    class ErrorManager
    {
        public:
            ErrorManager() = delete;
            static void GLClearError();
            static bool GLLogCall(const char* function, const char* file, int line);
    };
}