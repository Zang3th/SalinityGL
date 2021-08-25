#pragma once

#include "Mesh.hpp"
#include "Heightmap.hpp"
#include "ObjLoader.hpp"

namespace Core
{
    class MeshCreator
    {
        public:
            MeshCreator() = delete;
            static void CreatePlane(uint32 x, uint32 z, float tileSize, Mesh* mesh, const Heightmap* heightmap = nullptr);
            static void CreateFromObj(const std::string& filename, const std::string& baseFilepath, Mesh* mesh);
    };
}