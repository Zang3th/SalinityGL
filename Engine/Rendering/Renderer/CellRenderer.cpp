#include "CellRenderer.hpp"

namespace Engine
{
    // ----- Private -----

    CellRenderer::CellRenderer(Shader* shader, const glm::vec3& worldSpawnPos)
        :   _verticeCount(36), _shader(shader), _worldSpawnPos(worldSpawnPos), _positionStorage{glm::vec3(0.0f)},
            _colorStorage{glm::vec3(1.0f)}
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
        _vboVert = MakeScope<VertexBuffer>(CUBE_VERTICES_WITH_NORMALS, sizeof(CUBE_VERTICES_WITH_NORMALS), GL_STATIC_DRAW);
        _vao->DefineAttributes(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
        _vao->DefineAttributes(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

        _vboPos = MakeScope<VertexBuffer>(&_positionStorage[0], _positionStorage.size() * 3 * sizeof(float), GL_DYNAMIC_DRAW);
        _vao->DefineAttributes(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

        _vboColor = MakeScope<VertexBuffer>(&_colorStorage[0], _colorStorage.size() * 3 * sizeof(float), GL_DYNAMIC_DRAW);
        _vao->DefineAttributes(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

        //Set attribute divisors for instanced data
        _vao->AttributeDivisor(2, 1);
        _vao->AttributeDivisor(3, 1);

        //Unbind everything
        _vao->Unbind();
        _vboVert->Unbind();
        _vboPos->Unbind();
    }

    void CellRenderer::UploadGPUStorage()
    {
        _vboPos->Bind();
        _vboPos->Update(&_positionStorage[0], _positionStorage.size() * 3 * sizeof(float));
        _vboPos->Unbind();

        _vboColor->Bind();
        _vboColor->Update(&_colorStorage[0], _colorStorage.size() * 3 * sizeof(float));
        _vboColor->Unbind();
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
            _vboPos->Bind();
            _vboColor->Bind();

            //Set uniforms
            _shader->SetUniformMat4f("model", glm::mat4(1.0f));
            _shader->SetUniformMat4f("view", Camera3D::GetViewMatrix());
            _shader->SetUniformMat4f("projection", ((SceneRenderer*)sceneRenderer)->GetProjMatrix());
            _shader->SetUniformVec3f("viewPos", Camera3D::GetPosition());
            _shader->SetUniformVec3f("lightPos", LightParams::position);
            _shader->SetUniformVec3f("lightColor", LightParams::color);

            //Upload updated vbo's to the gpu
            UploadGPUStorage();

            //Render cells instanced
            GLCall(glDrawArraysInstanced(GL_TRIANGLES, 0, _verticeCount, cellCount))

            //Unbind vao and vbo's
            _vboColor->Unbind();
            _vboPos->Unbind();
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

    void CellRenderer::UpdateGPUStorage(uint32 index, const glm::u32vec3& cellPos, const glm::vec3& cellColor)
    {
        _positionStorage.at(index) = _worldSpawnPos + glm::vec3(cellPos);
        _colorStorage.at(index) = cellColor;
    }
}