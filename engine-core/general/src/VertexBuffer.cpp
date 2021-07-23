#include "VertexBuffer.hpp"

namespace Core
{
    // ----- Public -----

    VertexBuffer::VertexBuffer(const void* data, unsigned int size)
    {
        GLCall(glGenBuffers(1, &_vboID));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, _vboID));
        GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
    }

    VertexBuffer::~VertexBuffer()
    {
        GLCall(glDeleteBuffers(1, &_vboID));
    }

    void VertexBuffer::Bind()
    {
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, _vboID));
    }

    void VertexBuffer::Unbind()
    {
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    }
}