#include "CellRenderer.hpp"

namespace Engine
{
    // ----- Private -----

    CellRenderer::CellRenderer(float cellSize, Shader* shader, const glm::vec3& worldSpawnPos)
        :   _cellSize(cellSize), _verticeCount(36), _shader(shader), _worldSpawnPos(worldSpawnPos), _modelViewStorage{glm::mat4(0.0f)}
    {
        Logger::Info("Created", "Renderer",__func__);
        InitGpuStorage();
    }

    void CellRenderer::InitGpuStorage()
    {
        //Create and bind vao
        _vao = MakeScope<VertexArray>();
        _vao->Bind();

        //Create vbo's, send it data and configure vao
        _vboVert = MakeScope<VertexBuffer>(CUBE_VERTICES, sizeof(CUBE_VERTICES), GL_STATIC_DRAW);
        _vao->DefineAttributes(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

        _vboModel = MakeScope<VertexBuffer>(&_modelViewStorage[0], _modelViewStorage.size() * 16 * sizeof(float), GL_DYNAMIC_DRAW);
        _vao->DefineAttributes(1, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), nullptr);
        _vao->DefineAttributes(2, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(4 * sizeof(float)));
        _vao->DefineAttributes(3, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(8 * sizeof(float)));
        _vao->DefineAttributes(4, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(12 * sizeof(float)));

        //Set attribute divisors for per instance data
        _vao->AttributeDivisor(1, 1);
        _vao->AttributeDivisor(2, 1);
        _vao->AttributeDivisor(3, 1);
        _vao->AttributeDivisor(4, 1);

        //Unbind everything
        _vao->Unbind();
        _vboVert->Unbind();
        _vboModel->Unbind();
    }

    void CellRenderer::UpdateGpuStorage()
    {
        _vboModel->Bind();
        _vboModel->Update(&_modelViewStorage[0], _modelViewStorage.size() * 16 * sizeof(float));
        _vboModel->Unbind();
    }

    // ----- Public -----

    void CellRenderer::Flush(Renderer* sceneRenderer)
    {
        uint32 cellCount = CellSimParams::cellsAlive;

        if(cellCount > 0)
        {
            //Bind shader
            _shader->Bind();

            //Bind vao and vbo's
            _vao->Bind();
            _vboVert->Bind();
            _vboModel->Bind();

            //Set uniforms
            _shader->SetUniformMat4f("view", Camera3D::GetViewMatrix());
            _shader->SetUniformMat4f("projection", ((SceneRenderer*)sceneRenderer)->GetProjMatrix());

            //Upload updated vbo's to the gpu
            UpdateGpuStorage();

            //Render cells instanced
            GLCall(glDrawArraysInstanced(GL_TRIANGLES, 0, _verticeCount, cellCount))

            //Unbind vao and vbo's
            _vboModel->Unbind();
            _vboVert->Unbind();
            _vao->Unbind();

            //Unbind shader
            _shader->Unbind();

            //Save stats
            RenderStatistics::drawnVertices += _verticeCount * cellCount;
            RenderStatistics::drawCalls++;
            RenderStatistics::cellPasses++;
        }
    }

    void CellRenderer::UpdateModelViewStorage(uint32 index, const glm::u32vec3& cellPos)
    {
        glm::mat4 model(1.0f);
        model = glm::translate(model, _worldSpawnPos + (glm::vec3)cellPos);
        model = glm::scale(model, glm::vec3(_cellSize));
        _modelViewStorage.at(index) = model;
    }
}