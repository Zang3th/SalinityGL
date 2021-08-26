#include "ObjLoader.hpp"
#include <iostream>

namespace Core
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
            LOG(ERROR) << "Failed:   Obj-File-Loading | " << objFullFilepath;
        else
            LOG(INFO) << "Loaded:   Obj-File | " << objFullFilepath
                      << " (Shapes: " << shapes.size()
                      << " | Materials: " << materials.size()
                      << " | Vertices: " << attrib.vertices.size()
                      << " | TexCoords: " << attrib.texcoords.size()
                      << " | Indices: " << shapes[0].mesh.indices.size()
                      << ")";

        //Additional error checking to catch edge cases
        if(!warn.empty())
            LOG(WARNING) << "Failed:   " << warn << " | " << objFullFilepath;
        if(!err.empty())
            LOG(ERROR) << "Failed:   " << err << " | " << objFullFilepath;

        //Iterate over all shapes
        for(uint32 i = 0; i < shapes.size(); i++)
        {
            //Create new mesh
            Mesh mesh;

            //Iterate over all indices
            for(const auto& index : shapes.at(i).mesh.indices)
            {
                //Get vertices
                mesh.vertices.emplace_back(glm::vec3(
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2]
                ));

                //Get normals
                mesh.normals.emplace_back(glm::vec3(
                    attrib.normals[3 * index.normal_index + 0],
                    attrib.normals[3 * index.normal_index + 1],
                    attrib.normals[3 * index.normal_index + 2]
                ));

                //Get texCoords
                mesh.texCoords.emplace_back(glm::vec2(
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    attrib.texcoords[2 * index.texcoord_index + 1]
                ));

                //Auto increment indices
                mesh.indices.push_back(mesh.indices.size());
            }

            //Check for diffuse texture
            if(!materials[i].diffuse_texname.empty())
            {
                //Load diffuse texture
                std::string textureFilepath = baseFilepath + "/" + materials[i].diffuse_texname;
                std::string textureName = filename + "Texture";
                ResourceManager::LoadTexture(textureName, textureFilepath);
                mesh.diffuseTexture = ResourceManager::GetTexture(textureName);
            }

            //Check for bump map
            if(!materials[i].bump_texname.empty())
            {
                //Load bump map
                std::string textureFilepath = baseFilepath + "/" + materials[i].bump_texname;
                std::string textureName = filename + "Texture";
                ResourceManager::LoadTexture(textureName, textureFilepath);
                mesh.bumpTexture = ResourceManager::GetTexture(textureName);
            }

            //Check for blend mask
            if(!materials[i].alpha_texname.empty())
            {
                //Load blend mask
                std::string textureFilepath = baseFilepath + "/" + materials[i].alpha_texname;
                std::string textureName = filename + "Texture";
                ResourceManager::LoadTexture(textureName, textureFilepath);
                mesh.blendMask = ResourceManager::GetTexture(textureName);
            }

            //Copy mesh into meshes vector
            meshes->push_back(mesh);
        }
    }
}