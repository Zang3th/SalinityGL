#include "VertexArray.hpp"

namespace Core
{
    // ----- Public -----

    VertexArray::VertexArray()
    {
        GLCall(glGenVertexArrays(1, &_vaoID));
    }

    VertexArray::~VertexArray()
    {
        GLCall(glDeleteVertexArrays(1, &_vaoID));
    }

    void VertexArray::Bind()
    {
        GLCall(glBindVertexArray(_vaoID));
    }

    void VertexArray::Unbind()
    {
        GLCall(glBindVertexArray(0));
    }

    void VertexArray::DefineAttributes(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* offset)
    {
        GLCall(glVertexAttribPointer(index, size, type, normalized, stride, offset));
        GLCall(glEnableVertexAttribArray(index));
    }

    void VertexArray::AttributeDivisor(GLuint index, GLuint divisor)
    {
        GLCall(glVertexAttribDivisor(index, divisor));
    }
}