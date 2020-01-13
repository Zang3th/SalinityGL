#pragma once

#include "OpenGLErrorManager.hpp"

class VertexBuffer
{
private:
	unsigned int _RendererID;

public:
	VertexBuffer(const void* data, unsigned int size)
	{
		GLCall(glGenBuffers(1, &_RendererID));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, _RendererID));
		GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
	}

	~VertexBuffer()
	{
		GLCall(glDeleteBuffers(1, &_RendererID));
	}

	void bind() const
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, _RendererID));
	}

	void unbind() const
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}
};