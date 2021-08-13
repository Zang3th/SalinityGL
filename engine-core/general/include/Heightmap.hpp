#pragma once

#include "stb_image.hpp"
#include "CoreDefines.hpp"
#include "ErrorManager.hpp"

namespace Core
{
    class Heightmap
    {
        private:
            float _heightArray[PLANE_SIZE][PLANE_SIZE];

        public:
            Heightmap(const std::string& filepath);
            float GetValueAtF(unsigned int x, unsigned int z) const;
    };
}