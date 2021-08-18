#include "MeshCreator.hpp"

namespace Core
{
    // ----- Public -----

    void MeshCreator::CreatePlane(const unsigned int x, const unsigned int z, const float tileSize, const float edgeDepth, Mesh* mesh, const Heightmap* heightmap)
    {
        for (unsigned int j = 0; j <= z; ++j)
        {
            for (unsigned int i = 0; i <= x; ++i)
            {
                auto xPos = (float)i;
                auto yPos = 0.0f;
                auto zPos = (float)j;

                if(heightmap)
                    yPos = ((float)heightmap->GetValueAt(i, j) * 5) - 12.0f;

                //Bend edges down
                if(i == 0 || i == x || j == 0 || j == z)
                    yPos = edgeDepth;

                mesh->vertices.emplace_back(glm::vec3(xPos * tileSize, yPos, zPos * tileSize));
                mesh->texCoords.emplace_back(glm::vec2(xPos, zPos));
                mesh->normals.emplace_back(glm::vec3(0.0f, 1.0f, 0.0f));

                if ((j != z) && (i != x))
                {
                    unsigned int row1 = j * (x + 1);
                    unsigned int row2 = (j + 1) * (x + 1);

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