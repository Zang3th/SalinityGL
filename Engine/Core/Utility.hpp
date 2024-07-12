#pragma once

#include "glm.hpp"
#include "ColorLUT.hpp"

#include <cmath>

namespace Engine
{
    class Utility
    {
        public:
            Utility() = delete;
            static glm::vec3 GetColor_Scientic(float value, float min, float max);
            static glm::vec3 GetColor_BlackBody(float scalar);
            static glm::vec3 GetColor_ParaView(float scalar);
            static glm::vec3 TransformVec3uTo3f(const glm::uvec3& vec);
    };
}
