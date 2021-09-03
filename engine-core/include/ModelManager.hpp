#pragma once

#include "Model.hpp"
#include "ShadowRenderer.hpp"
#include "Heightmap.hpp"
#include "MeshCreator.hpp"
#include "ResourceManager.hpp"

#include <vector>

namespace Core
{
    class ModelManager
    {
        private:
            static        ShadowRenderer*     _shadowRenderer;
            inline static std::vector<Model*> _modelStorage = std::vector<Model*>();

        public:
            ModelManager() = delete;
            static void Init(ShadowRenderer* shadowRenderer);
            static Model* AddTerrain
            (
                uint32 x,
                uint32 z,
                float  tileSize,
                const std::string& name,
                const std::string& textureFilepath,
                const std::string& heightmapFilepath
            );
            static Model* AddPlane
            (
                uint32 x,
                uint32 z,
                float  tileSize,
                const std::string& name,
                const std::string& textureFilepath
            );
            static std::vector<Model*> AddObject(const std::string& filename, const std::string& baseFilepath);
            static void CleanUp();
    };
}