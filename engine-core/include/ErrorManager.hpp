#pragma once

#include "glad.hpp"
#include "Logger.hpp"
#include "CoreDefines.hpp"

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
            static bool GLLogCall(const char* function, const char* file, uint32 line);
            static void OpenGLMessageCallback
            (
                unsigned source,
                unsigned type,
                unsigned id,
                unsigned severity,
                int length,
                const char* message,
                const void* userParam
            );
    };
}