#include "MeshCreator.hpp"

namespace Core
{
    // ----- Private -----

    void MeshCreator::CalculateNormals(Mesh* mesh)
    {
        //Iterate over all indices
        for (uint32 i = 0; i < mesh->indices.size() - 2; i++)
        {
            //Get all indices of one triangle
            uint32 index0 = mesh->indices.at(i + 0);
            uint32 index1 = mesh->indices.at(i + 1);
            uint32 index2 = mesh->indices.at(i + 2);

            //Get all points of one triangle
            glm::vec3 point0 = mesh->vertices.at(index0);
            glm::vec3 point1 = mesh->vertices.at(index1);
            glm::vec3 point2 = mesh->vertices.at(index2);

            //Calculate the u vector
            float u0 = point1.x - point0.x;
            float u1 = point1.y - point0.y;
            float u2 = point1.z - point0.z;
            glm::vec3 U(u0, u1, u2);

            //Calculate the v vector
            float v0 = point2.x - point0.x;
            float v1 = point2.y - point0.y;
            float v2 = point2.z - point0.z;
            glm::vec3 V(v0, v1, v2);

            //Calculate cross product
            glm::vec3 p = glm::cross(U, V);

            //Add normal to all precalculated normals of this vertice, normalize and switch y-axis
            float p0_x_n = mesh->normals.at(index0).x + p.x;
            float p0_y_n = mesh->normals.at(index0).y + p.y * (-1);
            float p0_z_n = mesh->normals.at(index0).z + p.z;
            glm::vec3 P0 = glm::normalize(glm::vec3(p0_x_n, p0_y_n, p0_z_n));

            float p1_x_n = mesh->normals.at(index1).x + p.x;
            float p1_y_n = mesh->normals.at(index1).y + p.y * (-1);
            float p1_z_n = mesh->normals.at(index1).z + p.z;
            glm::vec3 P1 = glm::normalize(glm::vec3(p1_x_n, p1_y_n, p1_z_n));

            float p2_x_n = mesh->normals.at(index2).x + p.x;
            float p2_y_n = mesh->normals.at(index2).y + p.y * (-1);
            float p2_z_n = mesh->normals.at(index2).z + p.z;
            glm::vec3 P2 = glm::normalize(glm::vec3(p2_x_n, p2_y_n, p2_z_n));

            //Save normals
            mesh->normals.at(index0) = P0;
            mesh->normals.at(index1) = P1;
            mesh->normals.at(index2) = P2;
        }
    }

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
                    yPos = (float)heightmap->GetValueAt(i, j);

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

        CalculateNormals(mesh);
    }

    void MeshCreator::CreateFromObj(const std::string& filename, const std::string& baseFilepath, std::vector<Mesh>* meshes)
    {
        ObjLoader::ObjToMesh(filename, baseFilepath, meshes);
    }
}