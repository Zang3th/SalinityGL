#pragma once

#include "OpenGLErrorManager.hpp"

class VertexArray 
{
private:
	unsigned int _RendererID;

public:
	VertexArray()
	{
		GLCall(glGenVertexArrays(1, &_RendererID));
	}

	~VertexArray()
	{
		GLCall(glDeleteVertexArrays(1, &_RendererID));
	}

	void bind() const
	{
		GLCall(glBindVertexArray(_RendererID));
	}

	void unbind() const
	{
		GLCall(glBindVertexArray(0));
	}

	void DefineAttributes(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* offset)
	{
		GLCall(glVertexAttribPointer(index, size, type, normalized, stride, offset));
		GLCall(glEnableVertexAttribArray(index));
	}

	void AttributeDivisor(GLuint index, GLuint divisor)
	{
		GLCall(glVertexAttribDivisor(index, divisor));
	}
};