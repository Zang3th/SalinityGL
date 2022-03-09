#pragma once

#include "ErrorManager.hpp"

namespace Engine
{
    class VertexBuffer
    {
        private:
            uint32 _vboID;

        public:
        VertexBuffer(const void* data, uint32 size);
            ~VertexBuffer();
            void Bind() const;
            void Unbind() const;
    };
}