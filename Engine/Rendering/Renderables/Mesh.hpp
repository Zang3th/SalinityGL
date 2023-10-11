#pragma once

#include "glm.hpp"
#include "Types.hpp"
#include "Texture.hpp"

#include <vector>

namespace Engine
{
    struct Mesh
    {
        public:
            std::vector<glm::vec3> vertices;
            std::vector<glm::vec2> texCoords;
            std::vector<glm::vec3> normals;
            std::vector<glm::vec3> tangents;
            std::vector<uint32>    indices;
            std::vector<Texture*>  textures;
            int32                  gotDiffuseTex = 0, gotNormalMap = 0;
    };
}