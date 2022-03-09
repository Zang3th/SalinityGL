#pragma once

#include "Model.hpp"
#include "ShadowRenderer.hpp"
#include "Heightmap.hpp"
#include "MeshCreator.hpp"
#include "ResourceManager.hpp"

#include <vector>

namespace Engine
{
    class ModelManager
    {
        private:
            inline static std::vector<Model*> _modelStorage   = std::vector<Model*>();

        public:
            ModelManager() = delete;
            static Model* AddTerrain
            (
                uint32              x,
                uint32              z,
                float               tileSize,
                const  std::string& name,
                const  std::string& textureFilepath,
                const  std::string& heightmapFilepath
            );
            static Model* AddPlane
            (
                uint32 x,
                uint32 z,
                float  tileSize
            );
            static std::vector<Model*> AddObject
            (
                const std::string& filename,
                const std::string& baseFilepath
            );
            static void CleanUp();
    };
}