#include "ErrorManager.hpp"

void ErrorManager::GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool ErrorManager::GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
        LOG(ERROR) << "OpenGL Error [" << error << "] in " << function << " at line " << line;
		return false;
	}
	return true;
}