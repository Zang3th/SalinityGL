#pragma once

#include "Renderer.hpp"
#include "glm.hpp"
#include "Sprite.hpp"
#include "Shader.hpp"
#include "GLRenderSettings.hpp"
#include "GlobalParams.hpp"
#include "ResourceManager.hpp"

namespace Engine
{
    class GridRenderer final : public Renderer
    {
        friend class RenderManager;

    private:
        uint32    _gridWidth, _gridHeight, _quadSize, _quadAmountTotal;
        glm::vec3 _defaultColor;
        Shader*   _shader;

        Scope<VertexArray>  _vao;
        Scope<VertexBuffer> _vboVert, _vboColor;
        glm::mat4           _orthoProj, _model;

        std::vector<glm::vec3> _colorStorage;

        void InitGpuStorage();
        void UpdateGpuStorage();

    public:
        void Flush(Renderer* renderer) override;
        void Set(uint32 x, uint32 y, const glm::vec3& color) const;
        void Reset(uint32 x, uint32 y) const;
        void SetScreen(const glm::vec3& color) const;
        void ClearScreen() const;

        GridRenderer(uint32 width, uint32 height, uint32 quadSize, const std::string& shader);
    };
}