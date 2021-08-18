#include "IndexBuffer.hpp"

namespace Core
{
    // ----- Public -----

    IndexBuffer::IndexBuffer(const void* data, unsigned int size)
        :   _iboID(0)
    {
        GLCall(glGenBuffers(1, &_iboID));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _iboID));
        GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
    }

    IndexBuffer::~IndexBuffer()
    {
        GLCall(glDeleteBuffers(1, &_iboID));
    }

    void IndexBuffer::Bind() const
    {
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _iboID));
    }

    void IndexBuffer::Unbind() const
    {
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    }
}