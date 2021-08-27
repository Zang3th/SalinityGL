#pragma once

#include "Mesh.hpp"
#include "Heightmap.hpp"
#include "ObjLoader.hpp"
#include "glm.hpp"

#include <vector>

namespace Core
{
    class MeshCreator
    {
        private:
            static void CalculateNormals(Mesh* mesh);

        public:
            MeshCreator() = delete;
            static void CreatePlane(uint32 x, uint32 z, float tileSize, Mesh* mesh, const Heightmap* heightmap = nullptr);
            static void CreateFromObj(const std::string& filename, const std::string& baseFilepath, std::vector<Mesh>* meshes);
    };
}