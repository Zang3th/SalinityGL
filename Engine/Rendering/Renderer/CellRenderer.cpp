#include "CellRenderer.hpp"

namespace Engine
{
    // ----- Private -----

    CellRenderer::CellRenderer
    (
        float cellSize, float nearPlane, float farPlane,
        Shader* shader, const glm::vec3& worldSpawnPos
    )
        :   _cellSize(cellSize), _nearPlane(nearPlane), _farPlane(farPlane), _verticeCount(36),
            _cellCount(0), _shader(shader), _worldSpawnPos(worldSpawnPos)
    {
        Logger::Info("Created", "Renderer",__func__);
        //GenerateAllCells();
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
        _vboModel->Update(&_modelViewStorage[0], _cellCount * 16 * sizeof(float));
        _vboModel->Unbind();
    }

    void CellRenderer::UpdateModelViewStorage()
    {
        glm::mat4 model(1.0f);

        for(uint32 i = 0; i < _cellCount; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, _cellStorage.at(i).position);
            model = glm::scale(model, glm::vec3(_cellSize));
            _modelViewStorage.at(i) = model;
        }
    }

    //Function only used for stress testing
    void CellRenderer::GenerateAllCells()
    {
        uint32 count = 0;

        //Get initial world position
        for(uint32 x = 0; x < AppSettings::CELL_FRAME_SIZE; x++)
        {
            for(uint32 y = 0; y < AppSettings::CELL_FRAME_SIZE; y++)
            {
                for(uint32 z = 0; z < AppSettings::CELL_FRAME_SIZE; z++)
                {
                    _cellStorage.at(count).position = _worldSpawnPos + glm::vec3(x, y, z);
                    count++;
                }
            }
        }

        _cellCount = count;

        //Sort depending on distance to camera
        std::sort(_cellStorage.begin(), _cellStorage.end(),
        [](const Cell& a, const Cell& b)
        {
            return glm::length(Camera3D::GetPosition() - a.position) < glm::length(Camera3D::GetPosition() - b.position);
        });

        UpdateModelViewStorage();
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
        UpdateGpuStorage();

        //Render cells instanced
        GLCall(glDrawArraysInstanced(GL_TRIANGLES, 0, _verticeCount, _cellCount));

        //Unbind vao and vbo's
        _vboModel->Unbind();
        _vboVert->Unbind();
        _vao->Unbind();

        //Unbind shader
        _shader->Unbind();

        //Save stats
        AppSettings::renderStats.drawnVertices += _verticeCount * _cellCount;
        AppSettings::renderStats.drawCalls++;
        AppSettings::renderStats.cellPasses++;
    }

    uint32 CellRenderer::GetAliveCellAmount()
    {
        return _cellCount;
    }

    void CellRenderer::SpawnCell(CellType cellType, int32 cellAmount, const glm::vec3& cellPos)
    {
        _cellStorage.at(_cellCount) = {cellType, _worldSpawnPos + cellPos};
        _cellCount++;

        UpdateModelViewStorage();
    }

    void CellRenderer::CalculateCellPhysics()
    {
        //ToDo: Implement
    }
}