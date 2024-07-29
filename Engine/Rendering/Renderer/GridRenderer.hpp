#pragma once

#include "Renderer.hpp"
#include "glm.hpp"
#include "Sprite.hpp"
#include "Shader.hpp"
#include "GLRenderSettings.hpp"
#include "GlobalParams.hpp"
#include "ResourceManager.hpp"
#include "Utility.hpp"

#include <vector>

namespace Engine
{
    class GridRenderer final : public Renderer
    {
        private:
            uint32    _gridWidth, _gridHeight, _quadSize, _quadAmountTotal;
            Shader*   _shader;
            glm::vec3 _defaultColor;

            Scope<VertexArray>  _vao;
            Scope<VertexBuffer> _vboVert, _vboColor;
            glm::mat4           _orthoProj, _model;

            std::vector<glm::vec3> _colorStorage;
            std::vector<GridPos>   _emptyCells;

            void InitGpuStorage();

        public:
            GridRenderer(uint32 width, uint32 height, uint32 quadSize, const std::string& shader);

            void SetDefaultColor(const glm::vec3& color);
            void Flush(Renderer* renderer) override;
            void UpdateGpuStorage() const;
            void Set(uint32 x, uint32 y, const glm::vec3& color);
            void SetArea(const glm::uvec2& pos, uint32 size, const glm::vec3& color);
            void AddTextureBufferSubsampled(const std::string& texBuffer, const glm::uvec2& pos, uint32 size);

            [[nodiscard]] std::vector<GridPos> GetEmptyCells() const;
    };
}  // namespace Engine
