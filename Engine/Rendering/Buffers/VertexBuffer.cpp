#include "VertexBuffer.hpp"

namespace Engine
{
    // ----- Public -----

    VertexBuffer::VertexBuffer(const void* data, const uint32 size)
        :   _vboID(0)
    {
        GLCall(glGenBuffers(1, &_vboID));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, _vboID));
        GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
    }

    VertexBuffer::~VertexBuffer()
    {
        GLCall(glDeleteBuffers(1, &_vboID));
    }

    void VertexBuffer::Bind() const
    {
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, _vboID));
    }

    void VertexBuffer::Unbind() const
    {
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    }
}