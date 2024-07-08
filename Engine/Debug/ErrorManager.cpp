#include "ErrorManager.hpp"

namespace Engine
{
    // ----- Public -----

	void ErrorManager::GLClearError()
	{
		while(glGetError() != GL_NO_ERROR)
        {
            // ...
        }
	}

	bool ErrorManager::GLLogCall(const char* function, const char* file, uint32 line)
	{
		while(GLenum error = glGetError())
		{
            Logger::Error("GL_LOG", "Error: " + std::to_string(error), function);
            Logger::Error("GL_LOG", "Line: "  + std::to_string(line), file);

			return false;
		}

		return true;
	}

	void ErrorManager::OpenGLMessageCallback
	(
        uint32      source,
        uint32      type,
        uint32      id,
        uint32      severity,
        int32       length,
        const char* message,
        const void* userParam
	)
	{
        switch (severity)
        {
            case GL_DEBUG_SEVERITY_HIGH:         Logger::Error("GL_DEBUG", "SEVERITY_HIGH",  message); return;
            case GL_DEBUG_SEVERITY_MEDIUM:       Logger::Warn ("GL_DEBUG", "SEVERITY_MED.", message);  return;
            case GL_DEBUG_SEVERITY_LOW:          Logger::Info ("GL_DEBUG", "SEVERITY_LOW", message);  return;
            case GL_DEBUG_SEVERITY_NOTIFICATION: Logger::Trace("GL_DEBUG", "NOTIFICATION", message);   return;
            default: return;
        }
	}
}
