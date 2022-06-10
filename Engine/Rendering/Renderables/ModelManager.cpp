#include "ModelManager.hpp"

namespace Engine
{
    // ----- Public -----

    Model* ModelManager::AddTerrain
    (
        const uint32       x,
        const uint32       z,
        const float        tileSize,
        const std::string& name,
        const std::string& textureFilepath,
        const std::string& heightmapFilepath
    )
    {
        //Create terrain mesh
        Mesh terrainMesh;
        Heightmap heightmap(heightmapFilepath);
        MeshCreator::CreatePlane(x, z, tileSize, &terrainMesh, &heightmap);

        //Allocate and assign textures
        ResourceManager::LoadTextureFromFile(name, textureFilepath);
        terrainMesh.texture1 = ResourceManager::GetTexture(name);

        //Create and store model
        Model* terrainModel = new Model(&terrainMesh);
        _modelStorage.push_back(terrainModel);

        return terrainModel;
    }

    Model* ModelManager::AddPlane
    (
        const uint32 x,
        const uint32 z,
        const float  tileSize
    )
    {
        //Create plane mesh
        Mesh planeMesh;
        MeshCreator::CreatePlane(x, z, tileSize, &planeMesh);

        //Create and store model
        Model* planeModel = new Model(&planeMesh);
        _modelStorage.push_back(planeModel);

        return planeModel;
    }

    std::vector<Model*> ModelManager::AddObject(const std::string& filename, const std::string& baseFilepath)
    {
        //Return vector
        std::vector<Model*> models;

        //Get meshes out of obj-File
        std::vector<Mesh> meshes;
        Engine::MeshCreator::CreateFromObj(filename, baseFilepath, &meshes);

        //Create models out of meshes and store these
        for(auto& mesh : meshes)
        {
            Model* objModel = new Model(&mesh);
            _modelStorage.push_back(objModel);
            models.push_back(objModel);
        }

        return models;
    }

    void ModelManager::CleanUp()
    {
        for(auto const& model : _modelStorage)
            delete model;
    }
}