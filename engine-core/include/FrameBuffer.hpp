#pragma once

#include "ErrorManager.hpp"
#include "CoreDefines.hpp"
#include "ResourceManager.hpp"

namespace Core
{
    class FrameBuffer
    {
        private:
            uint32          _fboID, _depthBufferID;
            Texture         *_texture, *_depthTexture;

        public:
            FrameBuffer();
            ~FrameBuffer();
            void Bind() const;
            void Bind(uint32 width, uint32 height) const;
            void Unbind() const;
            void SetColorAttachment0();
            void CreateTextureAttachment(uint32 width, uint32 height);
            void CreateDepthTextureAttachment(uint32 width, uint32 height);
            void CreateDepthBufferAttachment(uint32 width, uint32 height);
            void DeleteColorBufferAttachment() const;
            [[nodiscard]] Texture* GetTexture() const;
            [[nodiscard]] Texture* GetDepthTexture() const;
    };
}