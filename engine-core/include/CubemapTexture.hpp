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
            explicit CubemapTexture(const std::array<const char*, 6>& faces);
            ~CubemapTexture();
            void Bind() const;
            void Unbind() const;
    };
}