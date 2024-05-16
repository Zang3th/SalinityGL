#include "Utility.hpp"

namespace Engine
{
    // ----- Public -----

    glm::vec3 Utility::GetColor_Scientic(float value, const float min, const float max)
    {
        value = fminf(fmaxf(value, min), max - 0.0001f);
        const float diff = max - min;
        value = diff == 0.0f ? 0.5f : (value - min) / diff;
        const float divider = 0.25f;
        const uint32 number = (uint8)floorf(value / divider);
        const float pos     = (value - (float)number * divider) / divider;

        glm::vec3 retVec{0.0f};
        switch(number)
        {
            case 0 : retVec.x = 0.0f; retVec.y = pos;  retVec.z = 1.0f;       break;
            case 1 : retVec.x = 0.0f; retVec.y = 1.0f; retVec.z = 1.0f - pos; break;
            case 2 : retVec.x = pos;  retVec.y = 1.0f; retVec.z = 0.0f;       break;
            case 3 : retVec.x = 1.0f; retVec.y = 1.0f; retVec.z = 0.0f;       break;
            default: break;
        }

        return retVec;
    }

    glm::vec3 Utility::GetColor_BlackBody(float scalar)
    {
        const uint32 idx = (uint32)(scalar * (ColorLUT::COLOR_TABLE_SIZE - 1));
        const ColorEntry color = ColorLUT::BlackBody[idx];
        return {color.r, color.g, color.b};
    }

    glm::vec3 Utility::GetColor_ParaView(float scalar)
    {
        const uint32 idx = (uint32)(scalar * (ColorLUT::COLOR_TABLE_SIZE - 1));
        const ColorEntry color = ColorLUT::ParaView[idx];
        return {color.r, color.g, color.b};
    }
}