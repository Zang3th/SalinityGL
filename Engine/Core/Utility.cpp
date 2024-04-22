#include "Utility.hpp"

namespace Engine
{
    // ----- Public -----

    glm::vec3 Utility::GetScienticColor(float value, const float min, const float max)
    {
        value = fminf(fmaxf(value, min), max - 0.0001f);
        const float diff = max - min;
        value = diff == 0.0f ? 0.5f : (value - min) / diff;
        const float divider = 0.25f;
        const float number  = floorf(value / divider);
        const float pos     = (value - number * divider) / divider;

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
}