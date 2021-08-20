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
            static void CreatePlane(unsigned int x, unsigned int z, float tileSize, float edgeDepth, Mesh* mesh, const Heightmap* heightmap = nullptr);
            static void CreateFromGLTF(const std::string& filepath, Mesh* mesh);
    };
}