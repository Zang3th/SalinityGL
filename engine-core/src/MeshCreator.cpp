#include "MeshCreator.hpp"

namespace Core
{
    // ----- Public -----

    void MeshCreator::CreatePlane(const uint32 x, const uint32 z, const float tileSize, Mesh* mesh, const Heightmap* heightmap)
    {
        for (uint32 j = 0; j <= z; ++j)
        {
            for (uint32 i = 0; i <= x; ++i)
            {
                auto xPos = (float)i;
                auto yPos = 0.0f;
                auto zPos = (float)j;

                if(heightmap)
                    yPos = ((float)heightmap->GetValueAt(i, j) * 5);

                mesh->vertices.emplace_back(glm::vec3(xPos * tileSize, yPos, zPos * tileSize));
                mesh->texCoords.emplace_back(glm::vec2(xPos, zPos));
                mesh->normals.emplace_back(glm::vec3(0.0f, 1.0f, 0.0f));

                if ((j != z) && (i != x))
                {
                    uint32 row1 = j * (x + 1);
                    uint32 row2 = (j + 1) * (x + 1);

                    //Triangle 1
                    mesh->indices.emplace_back(row1 + i);
                    mesh->indices.emplace_back(row1 + i + 1);
                    mesh->indices.emplace_back(row2 + i + 1);

                    //Triangle 2
                    mesh->indices.emplace_back(row1 + i);
                    mesh->indices.emplace_back(row2 + i + 1);
                    mesh->indices.emplace_back(row2 + i);
                }
            }
        }
    }

    void MeshCreator::CreateFromObj(const std::string& filename, const std::string& baseFilepath, Mesh* mesh)
    {
        ObjLoader::ObjToMesh(filename, baseFilepath, mesh);
    }
}