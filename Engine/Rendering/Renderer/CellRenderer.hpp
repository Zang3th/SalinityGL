#pragma once

#include "Renderer.hpp"
#include "Types.hpp"
#include "glm.hpp"
#include "Shader.hpp"
#include "Camera3D.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "SceneRenderer.hpp"
#include "VerticeData.hpp"

#include <array>
#include <algorithm>

namespace Engine
{
    class CellRenderer final : public Renderer
    {
        friend class RenderManager;

        private:

            struct Cell
            {
                CellType type;
                glm::vec3 position;
            };

            Scope<VertexArray>                                  _vao;
            Scope<VertexBuffer>                                 _vboVert, _vboModel;

            float                                               _cellSize, _nearPlane, _farPlane;
            uint32                                              _verticeCount, _cellCount;
            Shader*                                             _shader;
            glm::vec3                                           _worldSpawnPos;

            std::array<Cell, AppSettings::MAX_CELL_AMOUNT>      _cellStorage;
            std::array<glm::mat4, AppSettings::MAX_CELL_AMOUNT> _modelViewStorage;

            CellRenderer
            (
                float cellSize, float nearPlane, float farPlane,
                Shader* shader, const glm::vec3& worldSpawnPos
            );
            ~CellRenderer() final = default;

            void InitGpuStorage();
            void UpdateGpuStorage();
            void UpdateModelViewStorage();
            void GenerateAllCells();

        public:
            void   Flush(Renderer* renderer) final;
            uint32 GetAliveCellAmount();
            void   SpawnCell(CellType cellType, int32 cellAmount, const glm::vec3& cellPos);
            void   CalculateCellPhysics();
    };
}