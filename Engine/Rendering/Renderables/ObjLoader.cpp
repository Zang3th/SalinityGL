#include "ObjLoader.hpp"

namespace Engine
{
    // ----- Public -----

    void ObjLoader::ObjToMesh(const std::string& filename, const std::string& baseFilepath, std::vector<Mesh>* meshes)
    {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;
        std::string objFullFilepath = baseFilepath + "/" + filename + ".obj";

        if(!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, objFullFilepath.c_str(), baseFilepath.c_str()))
            Logger::Error("Failed", "OBJ-Loading", objFullFilepath);
        else
        {
            std::string objInfo0 = "(Shapes: "    + std::to_string(shapes.size())          + ", Materials: " + std::to_string(materials.size())  + ")";
            std::string objInfo1 = "(Vertices: "  + std::to_string(attrib.vertices.size()) + ", TexCoords: " + std::to_string(attrib.texcoords.size()) +
                                   ", Indices: "  + std::to_string(shapes[0].mesh.indices.size()) + ")";
            Logger::Info("Loaded", "OBJ-Model", objFullFilepath);
            Logger::Info("", "", objInfo0);
            Logger::Info("", "", objInfo1);
        }

        //Additional error checking to catch edge cases
        if(!warn.empty())
            Logger::Warn("Warning", "tinyobj", warn);
        if(!err.empty())
            Logger::Error("Error", "tinyobj", err);

        //Iterate over all shapes
        for(uint32 i = 0; i < shapes.size(); i++)
        {
            //Create new mesh
            Mesh mesh;

            //Iterate over all indices
            for(const auto& index : shapes.at(i).mesh.indices)
            {
                //Get vertices
                mesh.vertices.emplace_back(
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2]
                );

                //Get normals
                mesh.normals.emplace_back(
                    attrib.normals[3 * index.normal_index + 0],
                    attrib.normals[3 * index.normal_index + 1],
                    attrib.normals[3 * index.normal_index + 2]
                );

                //Get texCoords
                mesh.texCoords.emplace_back(
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    attrib.texcoords[2 * index.texcoord_index + 1]
                );

                //Reserve space for tangents
                mesh.tangents.emplace_back(0.0f, 0.0f, 0.0f);

                //Auto increment indices
                mesh.indices.push_back(mesh.indices.size());
            }

            //Check for diffuse texture
            if(!materials[i].diffuse_texname.empty())
            {
                //Load diffuse texture
                std::string textureFilepath = baseFilepath + "/" + materials[i].diffuse_texname;
                std::string textureName = filename + "DiffuseTexture";
                ResourceManager::LoadTextureFromFile(textureName, textureFilepath);
                mesh.textures.push_back(ResourceManager::GetTexture(textureName));
            }

            //Check for normal map
            if(!materials[i].bump_texname.empty())
            {
                //Load bump map
                std::string textureFilepath = baseFilepath + "/" + materials[i].bump_texname;
                std::string textureName = filename + "NormalTexture";
                ResourceManager::LoadTextureFromFile(textureName, textureFilepath);
                mesh.textures.push_back(ResourceManager::GetTexture(textureName));
                mesh.gotNormalMap = 1;
            }

            //Calculate tangents
            if(mesh.gotNormalMap == 1)
            {
                MeshCreator::CalculateTangents(&mesh);
                Logger::Info("Created", "Tangents", filename);
            }

            //Copy mesh into meshes vector
            meshes->push_back(mesh);
        }
    }
}