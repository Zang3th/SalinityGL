#pragma once

#include "Renderer.hpp"
#include "Types.hpp"
#include "glm.hpp"
#include "Shader.hpp"
#include "Cell.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"

#include <array>

namespace Engine
{
    class CellRenderer final : public Renderer
    {
        friend class RenderManager;

        public: //TODO: Make private after prototyping
            Scope<VertexArray>           _vao;
            Scope<VertexBuffer>          _vboVert, _vboModel;

            glm::vec3 _worldPos;
            float     _cellSize, _nearPlane, _farPlane;

            std::array<Cell*,     AppSettings::CELL_CUBE_SIZE * 3> _cellStorage;
            std::array<glm::mat4, AppSettings::CELL_CUBE_SIZE * 3> _modelViewStorage;
            Shader*   _cellShader;

            CellRenderer
            (
                const glm::vec3& worldPos, float cellSize, float nearPlane, float farPlane
            );
            ~CellRenderer() final;

            void InitGpuStorage();
            void UpdateGpuStorage();
            void GenerateCells();

        public:
            void Flush(Renderer* renderer) final;
    };
}