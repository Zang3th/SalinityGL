#include "ErrorManager.hpp"

void ErrorManager::GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool ErrorManager::GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
        LOG(ERROR) << "\nOpenGL Error with code " << error << " in " << function << " at line " << line << "\n";
		return false;
	}
	return true;
}