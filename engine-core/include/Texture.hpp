#pragma once

#include "stb_image.hpp"
#include "ErrorManager.hpp"

namespace Core
{
    //Anisotropic filtering is implemented as an extension and isn't included in OpenGL Core
    #define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE

    class Texture
    {
        private:
            uint32 _textureID;

        public:
            Texture();
            ~Texture();
            void InitFromFile(const std::string& filepath);
            void InitAsDepthTexture(uint32 width, uint32 height);
            void Bind() const;
            void BindToSlot(uint32 slot) const;
            [[nodiscard]] uint32 GetTextureID() const;
            void Unbind() const;
    };
}