#pragma once

#include "stb_image.hpp"
#include "ErrorManager.hpp"
#include <vector>

namespace Core
{
    class CubemapTexture
    {
        private:
            unsigned int _textureID;

        public:
            CubemapTexture(std::vector<const char*>& faces);
            ~CubemapTexture();
            void Bind() const;
            void Unbind() const;
    };
}