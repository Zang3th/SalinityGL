#include "IndexBuffer.hpp"

// ----- Public -----

namespace Core
{
    IndexBuffer::IndexBuffer(const void* data, unsigned int size)
    {
        GLCall(glGenBuffers(1, &_iboID));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _iboID));
        GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
    }

    IndexBuffer::~IndexBuffer()
    {
        GLCall(glDeleteBuffers(1, &_iboID));
    }

    void IndexBuffer::Bind()
    {
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _iboID));
    }

    void IndexBuffer::Unbind()
    {
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    }
}