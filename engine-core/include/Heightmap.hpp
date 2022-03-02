#pragma once

#include "stb_image.hpp"
#include "CoreDefines.hpp"
#include "Logger.hpp"

#include <vector>

namespace Core
{
    class Heightmap
    {
        private:
            inline static std::vector<float> _heightmap = std::vector<float>();

        public:
            explicit Heightmap(const std::string& filepath);
            [[nodiscard]] float GetValueAt(uint32 x, uint32 z) const;
    };
}