#pragma once

#include "glm.hpp"
#include "Types.hpp"

#include <cmath>

namespace Engine
{
    class Utility
    {
        public:
            Utility() = delete;
            static glm::vec3 GetScienticColor(float value, float min, float max);
    };
}