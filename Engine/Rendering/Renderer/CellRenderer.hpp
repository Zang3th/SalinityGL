#pragma once

#include "Renderer.hpp"
#include "Types.hpp"
#include "glm.hpp"
#include "Shader.hpp"
#include "Camera3D.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "SceneRenderer.hpp"
#include "Cell.hpp"

#include <array>

namespace Engine
{
    class CellRenderer final : public Renderer
    {
        friend class RenderManager;

        private:
            Scope<VertexArray>                                          _vao;
            Scope<VertexBuffer>                                         _vboVert, _vboModel;

            float                                                       _cellSize, _nearPlane, _farPlane;
            uint32                                                      _verticeCount, _instanceCount;
            Shader*                                                     _shader;
            glm::vec3                                                   _worldPos;

            std::array<glm::mat4, AppSettings::CELL_CUBE_SIZE * 3>      _modelViewStorage;

            CellRenderer
            (
                float cellSize, float nearPlane, float farPlane,
                Shader* shader, const glm::vec3& worldPos
            );
            ~CellRenderer() final;

            void InitGpuStorage();
            void UpdateGpuStorage();
            void GenerateCells();

        public:
            void Flush(Renderer* renderer) final;
    };
}