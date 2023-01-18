#include "MeshCreator.hpp"

namespace Engine
{
    // ----- Public -----

    void MeshCreator::CalculateNormals(Mesh* mesh)
    {
        //Iterate over all indices
        for (uint32 i = 0; i < mesh->indices.size(); i += 3)
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

    void MeshCreator::CalculateTangents(Mesh* mesh)
    {
        //Iterate over all indices
        for(uint32 i = 0; i < mesh->indices.size(); i += 3)
        {
            //Get all indices of one triangle
            uint32 index0 = mesh->indices.at(i + 0);
            uint32 index1 = mesh->indices.at(i + 1);
            uint32 index2 = mesh->indices.at(i + 2);

            //Get all points of one triangle
            glm::vec3 point0 = mesh->vertices.at(index0);
            glm::vec3 point1 = mesh->vertices.at(index1);
            glm::vec3 point2 = mesh->vertices.at(index2);

            //Get all uvs of one triangle
            glm::vec2 uv0 = mesh->texCoords.at(index0);
            glm::vec2 uv1 = mesh->texCoords.at(index1);
            glm::vec2 uv2 = mesh->texCoords.at(index2);

            //Calculate delta positions
            glm::vec3 e1 = point1 - point0;
            glm::vec3 e2 = point2 - point1;

            //Calculate delta uvs
            float x1 = uv1.x - uv0.x;
            float x2 = uv2.x - uv0.x;
            float y1 = uv1.y - uv0.y;
            float y2 = uv2.y - uv0.y;

            //Calculate tangent
            float r           = 1.0f / (x1 * y2 - x2 + y1);
            glm::vec3 tangent = r * (e1 * y2 - e2 * y1);

            //Add tangent to all precalculated tangents of the points of this triangle
            mesh->tangents.at(index0) += tangent;
            mesh->tangents.at(index1) += tangent;
            mesh->tangents.at(index2) += tangent;
        }

        //Normalize each tangent
        for (uint32 i = 0; i < mesh->vertices.size(); i++)
        {
            mesh->tangents.at(i) = glm::normalize(mesh->tangents.at(i));
        }
    }

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

                mesh->vertices.emplace_back(xPos * tileSize, yPos, zPos * tileSize);
                mesh->texCoords.emplace_back(zPos, xPos); //Flip texture coordinates
                mesh->normals.emplace_back(0.0f, 1.0f, 0.0f);
                mesh->tangents.emplace_back(0.0f, 0.0f, 0.0f);

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
        CalculateTangents(mesh);
    }

    void MeshCreator::CreateFromObj(const std::string& filename, const std::string& baseFilepath, std::vector<Mesh>* meshes)
    {
        ObjLoader::ObjToMesh(filename, baseFilepath, meshes);
    }
}