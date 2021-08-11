#include "ErrorManager.hpp"

namespace Core
{
    // ----- Public -----

	void ErrorManager::GLClearError()
	{
		while (glGetError() != GL_NO_ERROR);
	}

	bool ErrorManager::GLLogCall(const char* function, const char* file, int line)
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
}