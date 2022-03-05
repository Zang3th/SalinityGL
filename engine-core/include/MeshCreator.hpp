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


        public:
            MeshCreator() = delete;
            static void CalculateNormals(Mesh* mesh);
            static void CalculateTangents(Mesh* mesh);
            static void CreatePlane(uint32 x, uint32 z, float tileSize, Mesh* mesh, const Heightmap* heightmap = nullptr);
            static void CreateFromObj(const std::string& filename, const std::string& baseFilepath, std::vector<Mesh>* meshes);
    };
}