#pragma once

#include "OpenGLErrorManager.hpp"

class VertexBuffer
{
private:
	unsigned int _RendererID;

public:
	VertexBuffer(const void* data, unsigned int size, bool isDynamic = false)
		: _RendererID(0)
	{		
		if(isDynamic)
		{
			GLCall(glGenBuffers(1, &_RendererID));
			GLCall(glBindBuffer(GL_ARRAY_BUFFER, _RendererID));
			GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW));
		}
		else
		{
			GLCall(glGenBuffers(1, &_RendererID));
			GLCall(glBindBuffer(GL_ARRAY_BUFFER, _RendererID));
			GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
		}		
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

	void updateData(const void* data, unsigned int size)
	{
		GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, size, data));
	}
};