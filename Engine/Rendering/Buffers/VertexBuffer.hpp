#pragma once

#include "ErrorManager.hpp"

namespace Engine
{
    class VertexBuffer
    {
        private:
            uint32 _vboID;

        public:
            VertexBuffer(const void* data, uint32 size, uint32 bufferType);
            ~VertexBuffer();
            void Bind()   const;
            void Unbind() const;
            void Update(const void* data, const uint32 size);
    };
}