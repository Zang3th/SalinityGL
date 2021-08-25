#pragma once

#include "Mesh.hpp"
#include "Heightmap.hpp"

namespace Core
{
    class MeshCreator
    {
        public:
            MeshCreator() = delete;
            static void CreatePlane(uint32 x, uint32 z, float tileSize, Mesh* mesh, const Heightmap* heightmap = nullptr);
            static void CreateFromOBJ(const std::string& filepath, Mesh* mesh);
    };
}