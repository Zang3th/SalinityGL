#include "ObjLoader.hpp"
#include <iostream>

namespace Core
{
    // ----- Public -----

    void ObjLoader::ObjToMesh(const std::string& filename, const std::string& baseFilepath, Mesh* mesh)
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
        if(shapes.size() != 1)
            LOG(ERROR) << "Failed:   More than 1 shape per file is not supported | " << objFullFilepath;

        //Copy attrib data into mesh vectors
        for(const auto& index : shapes[0].mesh.indices)
        {
            //Get vertices
            mesh->vertices.emplace_back(glm::vec3(
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            ));

            //Get normals
            mesh->normals.emplace_back(glm::vec3(
                attrib.normals[3 * index.normal_index + 0],
                attrib.normals[3 * index.normal_index + 1],
                attrib.normals[3 * index.normal_index + 2]
            ));

            //Get texCoords
            mesh->texCoords.emplace_back(glm::vec2(
                attrib.texcoords[2 * index.texcoord_index + 0],
                attrib.texcoords[2 * index.texcoord_index + 1] //Flip vertically
            ));

            //Auto increment indices
            mesh->indices.push_back(mesh->indices.size());
        }

        //Load diffuse texture
        std::string textureFilepath = baseFilepath + "/" + materials[0].diffuse_texname;
        std::string textureName = filename + "Texture";
        ResourceManager::LoadTexture(textureName, textureFilepath);
        mesh->diffuseTexture = ResourceManager::GetTexture(textureName);
    }
}