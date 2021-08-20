#pragma once

#include "stb_image.hpp"
#include "CoreDefines.hpp"
#include "easylogging++.hpp"

namespace Core
{
    class Heightmap
    {
        private:
            float _heightArray[PLANE_SIZE][PLANE_SIZE];

        public:
            explicit Heightmap(const std::string& filepath);
            [[nodiscard]] float GetValueAt(unsigned int x, unsigned int z) const;
    };
}