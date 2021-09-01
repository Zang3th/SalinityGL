#pragma once

#include "stb_image.hpp"
#include "ErrorManager.hpp"

namespace Core
{
    //Anisotropic filtering is implemented as an extension and isn't included in OpenGL Core
    #define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE

    class Texture
    {
        public:
            enum Texture_Attribute
            {
                DEPTH
            };

        private:
            uint32 _textureID;

            void InitFromFile(const std::string& filepath);
            void InitWithAttribute(uint32 width, uint32 height, Texture_Attribute attribute);

        public:
            explicit Texture(const std::string& filepath);
            Texture(uint32 width, uint32 height, Texture_Attribute attribute);
            ~Texture();
            void Bind() const;
            void BindToSlot(uint32 slot) const;
            [[nodiscard]] uint32 GetTextureID() const;
            void Unbind() const;
    };
}