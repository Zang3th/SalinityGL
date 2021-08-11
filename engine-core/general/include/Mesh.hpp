#pragma once

#include <vector>
#include <glm/glm.hpp>

namespace Core
{
    class Mesh
    {
        public:
            std::vector<glm::vec3> vertices;
            std::vector<glm::vec2> texCoords;
            std::vector<glm::uvec3> indices;
            std::vector<glm::vec3> normals;
    };
}