#include "MeshCreator.hpp"

namespace Core
{
    // ----- Public -----

    void MeshCreator::CreateTerrain(const unsigned int size, const float tileSize, Mesh* mesh, Heightmap* heightmap)
    {
        for (unsigned int j = 0; j <= size; ++j)
        {
            for (unsigned int i = 0; i <= size; ++i)
            {
                auto xPos = (float)i;
                auto yPos = 0.0f;
                auto zPos = (float)j;

                if(heightmap)
                    yPos = ((float)heightmap->GetValueAtF(i, j) * 5) - 12.0f;

                //Bend edges down
                if(i == 0 || i == size)
                    yPos = -10.0f;
                if(j == 0 || j == size)
                    yPos = -10.0f;

                mesh->vertices.emplace_back(glm::vec3(xPos * tileSize, yPos, zPos * tileSize));
                mesh->texCoords.emplace_back(glm::vec2(xPos, zPos));
                mesh->normals.emplace_back(glm::vec3(0.0f, 1.0f, 0.0f));

                if ((j != size) && (i != size))
                {
                    unsigned int row1 = j * (size + 1);
                    unsigned int row2 = (j + 1) * (size + 1);

                    //Triangle 1
                    mesh->indices.emplace_back(glm::uvec3(row1 + i, row1 + i + 1, row2 + i + 1));

                    //Triangle 2
                    mesh->indices.emplace_back(glm::uvec3(row1 + i, row2 + i + 1, row2 + i));
                }
            }
        }
    }

    void MeshCreator::CreateFromGLTF(const std::string& filepath, Mesh* mesh)
    {
        Loader loader;
        loader.GLTFToMesh(filepath, mesh);
    }
}