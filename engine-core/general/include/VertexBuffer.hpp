#pragma once

#include "ErrorManager.hpp"

namespace Core
{
    class VertexBuffer
    {
        private:
            unsigned int _vboID;

        public:
            VertexBuffer(const void* data, unsigned int size);
            ~VertexBuffer();
            void Bind();
            void Unbind();
    };
}