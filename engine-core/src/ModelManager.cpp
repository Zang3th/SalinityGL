#include "ModelManager.hpp"

namespace Core
{
    // ----- Private -----

    ShadowRenderer* ModelManager::_shadowRenderer;

    // ----- Public -----

    void ModelManager::Init(ShadowRenderer* shadowRenderer)
    {
        _shadowRenderer = shadowRenderer;
    }

    Model* ModelManager::AddTerrain
    (
        const uint32 x,
        const uint32 z,
        const float  tileSize,
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
        terrainMesh.texture2 = nullptr;
        terrainMesh.texture3 = _shadowRenderer->GetDepthTexture();
        terrainMesh.gotNormalMap = false;

        //Create and store model
        Model* terrainModel = new Model(&terrainMesh);
        _modelStorage.push_back(terrainModel);

        return terrainModel;
    }

    Model* ModelManager::AddPlaneWithTexture
    (
        const uint32 x,
        const uint32 z,
        const float  tileSize,
        const std::string& name,
        const std::string& textureFilepath
    )
    {
        //Create plane mesh
        Mesh planeMesh;
        MeshCreator::CreatePlane(x, z, tileSize, &planeMesh);

        //Allocate and assign textures
        ResourceManager::LoadTextureFromFile(name, textureFilepath);
        planeMesh.texture1 = ResourceManager::GetTexture(name);
        planeMesh.texture2 = nullptr;
        planeMesh.texture3 = _shadowRenderer->GetDepthTexture();
        planeMesh.gotNormalMap = false;

        //Create and store model
        Model* planeModel = new Model(&planeMesh);
        _modelStorage.push_back(planeModel);

        return planeModel;
    }

    Model* ModelManager::AddPlaneWithoutTexture
    (
        const uint32 x,
        const uint32 z,
        const float  tileSize
    )
    {
        //Create plane mesh
        Mesh planeMesh;
        MeshCreator::CreatePlane(x, z, tileSize, &planeMesh);

        //Set default textures
        planeMesh.texture1 = nullptr;
        planeMesh.texture2 = nullptr;
        planeMesh.texture3 = nullptr;
        planeMesh.gotNormalMap = false;

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
        Core::MeshCreator::CreateFromObj(filename, baseFilepath, &meshes);

        //Create models out of meshes and store these
        for(auto& mesh : meshes)
        {
            mesh.texture3 = _shadowRenderer->GetDepthTexture();
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