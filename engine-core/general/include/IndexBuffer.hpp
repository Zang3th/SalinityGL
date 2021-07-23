#pragma once

#include "ErrorManager.hpp"

namespace Core
{
    class IndexBuffer
    {
        private:
            unsigned int _iboID;

        public:
            IndexBuffer(const void* data, unsigned int size);
            ~IndexBuffer();
            void Bind();
            void Unbind();
    };
}