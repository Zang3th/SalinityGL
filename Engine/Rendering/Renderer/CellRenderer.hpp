#pragma once

#include "Renderer.hpp"
#include "Types.hpp"
#include "GlobalParams.hpp"
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
                uint32   id;
                uint32   amount;
                CellType type;
            };

            Scope<VertexArray>                                  _vao;
            Scope<VertexBuffer>                                 _vboVert, _vboModel;

            float                                               _cellSize;
            uint32                                              _verticeCount, _cellCount;
            Shader*                                             _shader;
            glm::vec3                                           _worldSpawnPos;

            Cell _cellStorage[CellSimParams::CELL_FRAME_SIZE][CellSimParams::CELL_FRAME_SIZE][CellSimParams::CELL_FRAME_SIZE];
            std::array<glm::mat4, CellSimParams::MAX_CELL_AMOUNT> _modelViewStorage;
            std::array<uint32, CellSimParams::MAX_CELL_AMOUNT>    _cellIndexStorage;

            CellRenderer(float cellSize, Shader* shader, const glm::vec3& worldSpawnPos);
            ~CellRenderer() final = default;

            void InitGpuStorage();
            void UpdateGpuStorage();
            void UpdateModelViewStorage(uint32 index, const glm::vec3& pos);
            void InitCellStorage();
            inline static uint32 GetIndexFromCoords(const glm::u32vec3& cellPos);
            inline static glm::vec3 GetCoordsFromIndex(uint32 index);

        public:
            void Flush(Renderer* renderer) final;
            [[nodiscard]] uint32 GetAliveCellAmount() const;
            void SpawnCell(CellType cellType, uint32 cellAmount, const glm::u32vec3& cellPos);
            void DeleteAllCells();
            void CalculateCellPhysics();
    };
}