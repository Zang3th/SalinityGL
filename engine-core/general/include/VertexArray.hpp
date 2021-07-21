#pragma once

#include "ErrorManager.hpp"

class VertexArray
{
    private:
        unsigned int _vaoID;

    public:
        VertexArray();
        ~VertexArray();
        void Bind();
        void Unbind();
        void DefineAttributes(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* offset);
        void AttributeDivisor(GLuint index, GLuint divisor);
};