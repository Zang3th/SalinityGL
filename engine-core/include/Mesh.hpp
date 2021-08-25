#pragma once

#include "glm.hpp"
#include "CoreDefines.hpp"

#include <vector>

namespace Core
{
    class Mesh
    {
        public:
            std::vector<glm::vec3>  vertices;
            std::vector<glm::vec2>  texCoords;
            std::vector<glm::vec3>  normals;
            std::vector<uint32>     indices;
    };
}