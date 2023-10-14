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
#include "CellStorage.hpp"

#include <array>
#include <algorithm>

namespace Engine
{
    class CellRenderer final : public Renderer
    {
        friend class RenderManager;

        private:
            Scope<VertexArray>                                  _vao;
            Scope<VertexBuffer>                                 _vboVert, _vboModel;

            float                                               _cellSize;
            uint32                                              _verticeCount, _cellCount;
            Shader*                                             _shader;
            glm::vec3                                           _worldSpawnPos;
            CellStorage                                         _cellStorage;

            std::array<glm::mat4, CellSimParams::MAX_CELL_AMOUNT> _modelViewStorage;
            std::array<uint32, CellSimParams::MAX_CELL_AMOUNT>    _cellIndexStorage;

            CellRenderer(float cellSize, Shader* shader, const glm::vec3& worldSpawnPos);
            ~CellRenderer() final = default;

            void InitGpuStorage();
            void UpdateGpuStorage();
            void UpdateModelViewStorage(uint32 index, const glm::u32vec3& cellPos);
            void MoveCellDown(uint32 index, const glm::u32vec3& cellPos, const glm::u32vec3& cellPosBelow);

        public:
            void Flush(Renderer* renderer) final;
            [[nodiscard]] uint32 GetAliveCellAmount() const;
            void SpawnCell(CellType cellType, const glm::u32vec3& cellPos);
            void DeleteAllCells();
            void CalculateCellPhysics();
            void PrintDebug();
    };
}