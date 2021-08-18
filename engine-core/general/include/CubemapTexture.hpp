#pragma once

#include "stb_image.hpp"
#include "ErrorManager.hpp"
#include <array>

namespace Core
{
    class CubemapTexture
    {
        private:
            unsigned int _textureID;

        public:
            explicit CubemapTexture(std::array<const char*, 6>& faces);
            ~CubemapTexture();
            void Bind() const;
            void Unbind() const;
    };
}