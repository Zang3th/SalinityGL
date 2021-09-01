#pragma once

#include "stb_image.hpp"
#include "CoreDefines.hpp"
#include "Logger.hpp"

namespace Core
{
    class Heightmap
    {
        private:
            float _heightArray[PLANE_SIZE][PLANE_SIZE];

        public:
            explicit Heightmap(const std::string& filepath);
            [[nodiscard]] float GetValueAt(uint16 x, uint16 z) const;
    };
}