#pragma once

#include "Mesh.hpp"
#include "Loader.hpp"
#include "Heightmap.hpp"

namespace Core
{
    class MeshCreator
    {
        public:
            MeshCreator() = delete;
            static void CreateTerrain(unsigned int size, float tileSize, Mesh* mesh, Heightmap* heightmap = nullptr);
            static void CreateFromGLTF(const std::string& filepath, Mesh* mesh);
    };
}