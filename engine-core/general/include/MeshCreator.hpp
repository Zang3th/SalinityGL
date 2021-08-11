#pragma once

#include "Mesh.hpp"
#include "Loader.hpp"

namespace Core
{
    class MeshCreator
    {
        public:
            MeshCreator() = delete;
            static void CreatePlane(unsigned int size, float tileSize, Mesh* mesh);
            static void CreateFromGLTF(const std::string& filepath, Mesh* mesh);
    };
}