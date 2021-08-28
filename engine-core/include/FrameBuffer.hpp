#pragma once

#include "ErrorManager.hpp"
#include "CoreDefines.hpp"
#include "ResourceManager.hpp"

namespace Core
{
    class FrameBuffer
    {
        private:
            uint32          _fboID;
            Texture*        _depthTexture;

        public:
            FrameBuffer();
            ~FrameBuffer();
            void Bind(uint32 width, uint32 height) const;
            void Unbind() const;
            void DeleteColorBufferAttachment() const;
            void CreateDepthTextureAttachment(uint32 width, uint32 height);
            [[nodiscard]] Texture* GetDepthTexture() const;
    };
}