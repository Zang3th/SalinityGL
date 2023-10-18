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

namespace Engine
{
    class CellRenderer final : public Renderer
    {
        friend class RenderManager;

        private:
            Scope<VertexArray>  _vao;
            Scope<VertexBuffer> _vboVert, _vboModel;

            float     _cellSize;
            uint32    _verticeCount;
            Shader*   _shader;
            glm::vec3 _worldSpawnPos;

            std::array<glm::mat4, CellSimParams::MAX_CELL_AMOUNT> _modelViewStorage;

            CellRenderer(float cellSize, Shader* shader, const glm::vec3& worldSpawnPos);
            ~CellRenderer() final = default;

            void InitGpuStorage();
            void UpdateGpuStorage();

        public:
            void Flush(Renderer* renderer) final;
            void UpdateModelViewStorage(uint32 index, const glm::u32vec3& cellPos);
    };
}