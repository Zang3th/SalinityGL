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
                uint32   amount;
            };

            Scope<VertexArray>                                  _vao;
            Scope<VertexBuffer>                                 _vboVert, _vboModel;

            float                                               _cellSize, _nearPlane, _farPlane;
            uint32                                              _verticeCount, _cellCount;
            Shader*                                             _shader;
            glm::vec3                                           _worldSpawnPos;

            Cell _cellStorage[AppSettings::CELL_FRAME_SIZE][AppSettings::CELL_FRAME_SIZE][AppSettings::CELL_FRAME_SIZE];
            std::array<glm::mat4, AppSettings::MAX_CELL_AMOUNT> _modelViewStorage;

            CellRenderer
            (
                float cellSize, float nearPlane, float farPlane,
                Shader* shader, const glm::vec3& worldSpawnPos
            );
            ~CellRenderer() final = default;

            void InitGpuStorage();
            void UpdateGpuStorage();
            void UpdateModelViewStorage(uint32 index, const glm::vec3& pos);
            void InitCellStorage();

        public:
            void Flush(Renderer* renderer) final;
            [[nodiscard]] uint32 GetAliveCellAmount() const;
            void SpawnCell(CellType cellType, uint32 cellAmount, const glm::u32vec3& cellPos);
            void DeleteAllCells();
            void CalculateCellPhysics();
    };
}