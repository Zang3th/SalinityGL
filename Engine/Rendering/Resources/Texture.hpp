#pragma once

#include "stb_image.hpp"
#include "ErrorManager.hpp"

namespace Engine
{
    //Anisotropic filtering is implemented as an extension and isn't included in the OpenGL-Core
    #define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE

    class Texture
    {
        private:
            uint32 _textureID, _numberOfRows;

            void InitFromFile(const std::string& filepath);
            void Init(uint32 width, uint32 height, GLint internalFormat, GLenum format, GLenum type);

        public:
            explicit Texture(const std::string& filepath);
            Texture(const std::string& filepath, uint32 numberOfRows);
            Texture(uint32 width, uint32 height, GLint internalFormat, GLenum format, GLenum type);
            ~Texture();

            void Bind()                            const;
            void BindToSlot(uint32 slot)           const;
            void Unbind()                          const;
            void ActivateTextureFlipOnLoad()       const;
            void DeactivateTextureFlipOnLoad()     const;
            void AddFilterNearest()                const;
            void AddFilterLinear()                 const;
            void AddWrapRepeat()                   const;
            void ClampToEdge()                     const;
            void AddBorderColor()                  const;

            [[nodiscard]] uint32 GetTextureID()    const;
            [[nodiscard]] uint32 GetNumberOfRows() const;
    };
}