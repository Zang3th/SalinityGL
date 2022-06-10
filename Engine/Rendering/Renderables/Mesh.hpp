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
            std::vector<glm::vec3>  vertices;
            std::vector<glm::vec2>  texCoords;
            std::vector<glm::vec3>  normals;
            std::vector<glm::vec3>  tangents;
            std::vector<uint32>     indices;
            Texture                *texture1, *texture2, *texture3 , *texture4, *texture5;
            int32                   gotNormalMap;

            Mesh()
                : texture1(nullptr), texture2(nullptr), texture3(nullptr), texture4(nullptr), texture5(nullptr),
                  gotNormalMap(0)
            {};
    };
}