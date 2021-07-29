#pragma once

#include "ErrorManager.hpp"

namespace Core
{
    class VertexArray
    {
        private:
            unsigned int _vaoID;

        public:
            VertexArray();
            ~VertexArray();
            void Bind() const;
            void Unbind() const;
            void DefineAttributes(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* offset);
            void AttributeDivisor(GLuint index, GLuint divisor);
    };
}