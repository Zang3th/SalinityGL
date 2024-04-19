#pragma once

#include "stb_image.hpp"
#include "ErrorManager.hpp"
#include "glm.hpp"

#include <cstring>

namespace Engine
{
    //Anisotropic filtering is implemented as an extension and isn't included in the OpenGL-Core
    #define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE

    class Texture
    {
        private:
            int32          _width, _height, _nrChannels;
            GLenum         _format;
            uint32         _textureID, _numberOfRows;
            bool           _saveToBuffer;
            unsigned char *_imgBuffer, *_backupBuffer;

            void InitFromFile(const std::string& filepath);
            void Create(uint32 width, uint32 height, GLint internalFormat, GLenum format, GLenum type);

        public:
            explicit Texture(const std::string &filepath, uint32 numberOfRows = 0, bool saveToBuffer = false);
            Texture(int32 width, int32 height, GLint internalFormat, GLenum format, GLenum type);
            ~Texture();

            void Bind() const;
            void BindToSlot(uint32 slot) const;
            void Unbind() const;
            void ActivateTextureFlipOnLoad() const;
            void DeactivateTextureFlipOnLoad() const;
            void AddFilterNearest() const;
            void AddFilterLinear() const;
            void AddWrapRepeat() const;
            void ClampToEdge() const;
            void AddBorderColor() const;
            void ModifyTexture(uint32 x, uint32 y, const glm::vec3& color);
            void ResetTextureModification(uint32 x, uint32 y);

            [[nodiscard]] uint32 GetTextureID() const;
            [[nodiscard]] uint32 GetNumberOfRows() const;
    };
}