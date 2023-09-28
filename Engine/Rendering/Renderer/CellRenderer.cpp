#include "CellRenderer.hpp"

namespace Engine
{
    // ----- Private -----

    CellRenderer::CellRenderer
    (
        float cellSize, float nearPlane, float farPlane,
        Shader* shader, const glm::vec3& worldPos
    )
        :   _cellSize(cellSize), _nearPlane(nearPlane), _farPlane(farPlane), _verticeCount(36),
            _instanceCount(AppSettings::MAX_CELL_AMOUNT), _shader(shader), _worldPos(worldPos)
    {
        Logger::Info("Created", "Renderer",__func__);
        GenerateCells();
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

    void CellRenderer::GenerateCells()
    {
        glm::mat4 model(1.0f);
        uint32 count = 0;

        for(uint32 x = 0; x < AppSettings::CELL_FRAME_SIZE; x++)
        {
            for(uint32 y = 0; y < AppSettings::CELL_FRAME_SIZE; y++)
            {
                for(uint32 z = 0; z < AppSettings::CELL_FRAME_SIZE; z++)
                {
                    //Set transformations
                    model = glm::mat4(1.0f);
                    model = glm::translate(model, _worldPos + glm::vec3(x, y, z));
                    model = glm::scale(model, glm::vec3(_cellSize));
                    _modelViewStorage.at(count) = model;
                    count++;
                }
            }
        }
    }

    // ----- Public -----

    void CellRenderer::Flush(Renderer* sceneRenderer)
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
        //UpdateGpuStorage();

        //Render cells instanced
        GLCall(glDrawArraysInstanced(GL_TRIANGLES, 0, _verticeCount, _instanceCount));

        //Unbind vao and vbo's
        _vboModel->Unbind();
        _vboVert->Unbind();
        _vao->Unbind();

        //Unbind shader
        _shader->Unbind();

        //Save stats
        AppSettings::renderStats.drawnVertices += _verticeCount * _instanceCount;
        AppSettings::renderStats.drawCalls++;
        AppSettings::renderStats.cellPasses++;
    }
}