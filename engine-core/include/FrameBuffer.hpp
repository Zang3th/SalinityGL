#pragma once

#include "ErrorManager.hpp"
#include "CoreDefines.hpp"

namespace Core
{
    class FrameBuffer
    {
        private:
            unsigned int _fboID;
            Scope<unsigned int> _textureID, _depthTextureID, _renderBufferID;

        public:
            FrameBuffer();
            ~FrameBuffer();
            void Bind(unsigned int width, unsigned int height) const;
            void Unbind() const;
            void CreateTextureAttachment(unsigned int width, unsigned int height);
            void CreateDepthTextureAttachment(unsigned int width, unsigned int height);
            void CreateRenderBufferAttachment(unsigned int width, unsigned int height);
    };
}