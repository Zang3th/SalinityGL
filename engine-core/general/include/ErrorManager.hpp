#pragma once

#include "../../vendor/include/glad.hpp"
#include "../../vendor/include/easylogging++.hpp"
#include <signal.h>

#define ASSERT(x) if(!(x)) raise(SIGTRAP);
#define GLCall(x) ErrorManager::GLClearError();\
x;\
ASSERT(ErrorManager::GLLogCall(#x, __FILE__, __LINE__))

class ErrorManager
{
    private:
        ErrorManager(){}
        
    public:
        static void GLClearError();
        static bool GLLogCall(const char* function, const char* file, int line);
};