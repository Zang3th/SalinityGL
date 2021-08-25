#pragma once

#include "ErrorManager.hpp"

namespace Core
{
    class IndexBuffer
    {
        private:
            uint32 _iboID;

        public:
            IndexBuffer(const void* data, uint32 size);
            ~IndexBuffer();
            void Bind() const;
            void Unbind() const;
    };
}