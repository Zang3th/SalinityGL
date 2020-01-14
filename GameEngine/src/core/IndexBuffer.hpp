#pragma once

#include "OpenGLErrorManager.hpp"

class IndexBuffer
{
private:
	unsigned int _RendererID;

public:
	IndexBuffer(const void* data, unsigned int size)
	{
		GLCall(glGenBuffers(1, &_RendererID));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _RendererID));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
	}

	~IndexBuffer()
	{
		GLCall(glDeleteBuffers(1, &_RendererID));
	}

	void bind() const
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _RendererID));
	}

	void unbind() const
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}
};