#include "ErrorManager.hpp"

namespace Engine
{
    // ----- Public -----

	void ErrorManager::GLClearError()
	{
		while (glGetError() != GL_NO_ERROR);
	}

	bool ErrorManager::GLLogCall(const char* function, const char* file, uint32 line)
	{
		while (GLenum error = glGetError())
		{
			LOG(ERROR) << "OpenGL Error [" << error << "]";
			LOG(ERROR) << "File: " << file;
			LOG(ERROR) << "Function: " << function << " at line [" << line << "]";

			return false;
		}
		return true;
	}

	void ErrorManager::OpenGLMessageCallback
	(
	    unsigned source,
	    unsigned type,
	    unsigned id,
	    unsigned severity,
	    int length,
	    const char* message,
	    const void* userParam
	)
	{
        switch (severity)
        {
            case GL_DEBUG_SEVERITY_HIGH:         LOG(ERROR)     << message; return;
            case GL_DEBUG_SEVERITY_MEDIUM:       LOG(WARNING)   << message; return;
            case GL_DEBUG_SEVERITY_LOW:          LOG(INFO)      << message; return;
            case GL_DEBUG_SEVERITY_NOTIFICATION: LOG(TRACE)     << message; return;
            default: return;
        }
	}
}