#pragma once

#include "TextureBuffer.hpp"
#include "ErrorManager.hpp"

namespace Engine
{
    //Anisotropic filtering is implemented as an extension and isn't included in the OpenGL-Core
    #define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE

    class GLTexture
    {
        private:
            uint32               _initStatus, _width, _height, _channels, _textureID, _numberOfRows;
            GLenum               _format;
            bool                 _ownsTexBufPointer;
            const TextureBuffer* _textureBuffer;

            [[nodiscard]] uint32 Init();
            [[nodiscard]] uint32 Create(GLint internalFormat, GLenum type);

        public:
            explicit GLTexture(const TextureBuffer* texBuffer);
            explicit GLTexture(const std::string& filepath, bool saveBackup, uint32 numberOfRows = 0);
            GLTexture(uint32 width, uint32 height, GLint internalFormat, GLenum format, GLenum type);
            ~GLTexture();

            void Bind() const;
            void BindToSlot(uint32 slot) const;
            void Unbind() const;

            void AddMinFilterMipmapLinear() const;
            void AddMinFilterNearest() const;
            void AddMaxFilterNearest() const;
            void AddMinFilterLinear() const;
            void AddMaxFilterLinear() const;
            void AddWrapRepeat() const;
            void AddClampToEdge() const;
            void AddBorderColor(const glm::vec4& color) const;

            [[nodiscard]] uint32               GetWidth() const;
            [[nodiscard]] uint32               GetHeight() const;
            [[nodiscard]] uint32               GetTextureID() const;
            [[nodiscard]] uint32               GetNumberOfRows() const;
            [[nodiscard]] const TextureBuffer* GetTextureBuffer() const;
    };
}
