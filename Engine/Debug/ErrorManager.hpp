#pragma once

#include "glad/gl.hpp"
#include "Logger.hpp"
#include "Types.hpp"

#include <csignal>

#define ASSERT(x) if(!(x)) raise(SIGTRAP);
#define GLCall(x) ErrorManager::GLClearError();\
x;\
ASSERT(ErrorManager::GLLogCall(#x, __FILE__, __LINE__))

namespace Engine
{
    class ErrorManager
    {
        public:
            ErrorManager() = delete;
            static void GLClearError();
            static bool GLLogCall(const char* function, const char* file, uint32 line);
            static void OpenGLMessageCallback
            (
                uint32      source,
                uint32      type,
                uint32      id,
                uint32      severity,
                int32       length,
                const char* message,
                const void* userParam
            );
    };
}