#pragma once

#include <GL/glew.h>
#include <spdlog/spdlog.h>
#include <iostream>

#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
 x;\
 ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "\n";
		spdlog::error("---- OpenGL Error ----");
		std::cout << "Errorcode: " << error << " | " << function << " | Line: " << line;
		std::cout << "\n";
		return false;
	}
	return true;
}