#include "Mesh.hpp"

namespace Core
{
    // ----- Public -----

    void Mesh::CreatePlane(const unsigned int size, const float tileSize)
    {
        for (unsigned int j = 0; j <= size; ++j)
        {
            for (unsigned int i = 0; i <= size; ++i)
            {
                auto xPos = (float)i;
                auto yPos = 0.0f;
                auto zPos = (float)j;

                vertices.emplace_back(glm::vec3(xPos * tileSize, yPos, zPos * tileSize));
                texCoords.emplace_back(glm::vec2(xPos, zPos));
                normals.emplace_back(glm::vec3(0.0f, 1.0f, 0.0f));

                if ((j != size) && (i != size))
                {
                    unsigned int row1 = j * (size + 1);
                    unsigned int row2 = (j + 1) * (size + 1);

                    //Triangle 1
                    indices.emplace_back(glm::uvec3(row1 + i, row1 + i + 1, row2 + i + 1));

                    //Triangle 2
                    indices.emplace_back(glm::uvec3(row1 + i, row2 + i + 1, row2 + i));
                }
            }
        }
    }
}