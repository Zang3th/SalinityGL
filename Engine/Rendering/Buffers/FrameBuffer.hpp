#pragma once

#include "ErrorManager.hpp"
#include "Types.hpp"
#include "GlobalParams.hpp"
#include "ResourceManager.hpp"
#include "GLRenderSettings.hpp"

namespace Engine
{
    class FrameBuffer
    {
        private:
            uint32          _fboID,    _depthBufferID;
            GLTexture      *_texture, *_depthTexture;

        public:
            FrameBuffer();
            ~FrameBuffer();
            void Bind() const;
            void Bind(uint32 width, uint32 height) const;
            void Unbind() const;
            void SetColorAttachment0();
            GLTexture* CreateTextureAttachment(const std::string& name, uint32 width, uint32 height);
            GLTexture* CreateDepthTextureAttachment(const std::string& name, uint32 width, uint32 height);
            void CreateDepthBufferAttachment(uint32 width, uint32 height);
            void DeleteColorBufferAttachment() const;
            [[nodiscard]] GLTexture* GetTexture() const;
            [[nodiscard]] GLTexture* GetDepthTexture() const;
    };
}
