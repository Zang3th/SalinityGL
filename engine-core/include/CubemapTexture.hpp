#pragma once

#include "stb_image.hpp"
#include "ErrorManager.hpp"
#include "CoreDefines.hpp"

#include <array>

namespace Core
{
    class CubemapTexture
    {
        private:
            uint32 _textureID;

        public:
            explicit CubemapTexture(std::array<const std::string, 6>& faces);
            ~CubemapTexture();
            void Bind() const;
            void Unbind() const;
    };
}