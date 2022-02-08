#pragma once

#include "stb_image.hpp"
#include "CoreDefines.hpp"
#include "Logger.hpp"

namespace Core
{
    class Heightmap
    {
        private:
            float* _heightArray = nullptr;

        public:
            explicit Heightmap(const std::string& filepath);
            ~Heightmap();
            [[nodiscard]] float GetValueAt(uint32 x, uint32 z) const;
    };
}