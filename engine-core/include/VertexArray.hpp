#pragma once

#include "ErrorManager.hpp"

namespace Core
{
    class VertexArray
    {
        private:
            uint32 _vaoID;

        public:
            VertexArray();
            ~VertexArray();
            void Bind() const;
            void Unbind() const;
            static void DefineAttributes(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* offset);
            static void AttributeDivisor(GLuint index, GLuint divisor);
    };
}