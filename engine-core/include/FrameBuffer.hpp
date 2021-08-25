#pragma once

#include "ErrorManager.hpp"
#include "CoreDefines.hpp"

namespace Core
{
    class FrameBuffer
    {
        private:
            uint32          _fboID;
            Scope<uint32>   _textureID, _depthTextureID, _renderBufferID;

        public:
            FrameBuffer();
            ~FrameBuffer();
            void Bind(uint32 width, uint32 height) const;
            void Unbind() const;
            void CreateTextureAttachment(uint32 width, uint32 height);
            void CreateDepthTextureAttachment(uint32 width, uint32 height);
            void CreateRenderBufferAttachment(uint32 width, uint32 height);
    };
}