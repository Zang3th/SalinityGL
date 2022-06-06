#include "VertexBuffer.hpp"

namespace Engine
{
    // ----- Public -----

    VertexBuffer::VertexBuffer(const void* data, const uint32 size, uint32 bufferType)
        :   _vboID(0)
    {
        GLCall(glGenBuffers(1, &_vboID));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, _vboID));
        GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, bufferType));
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

    void VertexBuffer::Update(const void* data, const uint32 size)
    {
        GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, size, data));
    }
}