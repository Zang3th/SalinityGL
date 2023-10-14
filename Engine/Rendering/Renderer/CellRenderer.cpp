#include "CellRenderer.hpp"

namespace Engine
{
    // ----- Private -----

    CellRenderer::CellRenderer(float cellSize, Shader* shader, const glm::vec3& worldSpawnPos)
        :   _cellSize(cellSize), _verticeCount(36), _cellCount(0), _shader(shader), _worldSpawnPos(worldSpawnPos)
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

    void CellRenderer::UpdateModelViewStorage(uint32 index, const glm::u32vec3& cellPos)
    {
        glm::mat4 model(1.0f);
        model = glm::translate(model, _worldSpawnPos + (glm::vec3)cellPos);
        model = glm::scale(model, glm::vec3(_cellSize));
        _modelViewStorage.at(index) = model;
    }

    void CellRenderer::MoveCellDown(const uint32 index, const glm::u32vec3& cellPos, const glm::u32vec3& cellPosBelow)
    {
        //Move cell down (set values of the cell below to the values of the current cell)
        _cellStorage.Set(cellPosBelow, _cellStorage.Get(cellPos));

        //Delete old cell
        _cellStorage.Set(cellPos, {0, CellType::None});

        //Update the corresponding model view buffer for upload to the gpu
        UpdateModelViewStorage(_cellStorage.Get(cellPosBelow).id, cellPosBelow);

        //Update index
        _cellIndexStorage.at(index) = CellStorage::GetIndexFrom3DPos(cellPosBelow);
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
        GLCall(glDrawArraysInstanced(GL_TRIANGLES, 0, _verticeCount, _cellCount))

        //Unbind vao and vbo's
        _vboModel->Unbind();
        _vboVert->Unbind();
        _vao->Unbind();

        //Unbind shader
        _shader->Unbind();

        //Save stats
        RenderStatistics::drawnVertices += _verticeCount * _cellCount;
        RenderStatistics::drawCalls++;
        RenderStatistics::cellPasses++;
    }

    uint32 CellRenderer::GetAliveCellAmount() const
    {
        return _cellCount;
    }

    void CellRenderer::SpawnCell(CellType cellType, const glm::u32vec3& cellPos)
    {
        //Check for correct spawn type
        if(cellType != CellType::None)
        {
            //Check if cell is empty
            if(_cellStorage.Get(cellPos).type == CellType::None)
            {
                //Save cell in 3D array with type and amount
                _cellStorage.Set(cellPos, {_cellCount, cellType});

                //Save the index of the cell (how to directly access it in the 3d array)
                _cellIndexStorage.at(_cellCount) = CellStorage::GetIndexFrom3DPos(cellPos);

                //Update the corresponding model view buffer for upload to the gpu
                UpdateModelViewStorage(_cellCount, glm::vec3((float)cellPos.x, (float)cellPos.y, (float)cellPos.z));

                _cellCount++;
            }
        }
    }

    void CellRenderer::DeleteAllCells()
    {
        _cellStorage.Init();
        _cellCount = 0;
    }

    void CellRenderer::CalculateCellPhysics()
    {
        //Iterate over all cell indexes
        for(uint32 i = 0; i < _cellCount; i++)
        {
            //Get coordinates from current cell
            glm::u32vec3 cellPos = CellStorage::Get3DPosFromIndex(_cellIndexStorage.at(i));

            //Check if the cell is not touching the ground
            if(cellPos.y > 0)
            {
                //Get coordinates from cell below (y - 1)
                glm::u32vec3 cellPosBelow = glm::u32vec3(cellPos.x, cellPos.y - 1, cellPos.z);

                //Check if cell below is free
                if(_cellStorage.Get(cellPosBelow).type == CellType::None)
                {
                    MoveCellDown(i, cellPos, cellPosBelow);
                }
            }
        }
    }

    void CellRenderer::PrintDebug()
    {
        Logger::LineBreak();
        Logger::Print("Count | 3D-Index | (X, Y, Z)    | ID    | Type  |");

        for(uint32 i = 0; i < _cellCount; i++)
        {
            uint32 index = _cellIndexStorage.at(i);
            glm::u32vec3 cellPos = CellStorage::Get3DPosFromIndex(_cellIndexStorage.at(i));
            uint32 id = _cellStorage.Get(cellPos).id;
            CellType type = _cellStorage.Get(cellPos).type;

            Logger::Print(FileManager::PadString(std::to_string(i), 5) + " | "
                             + FileManager::PadString(std::to_string(index), 8) + " | "
                             + FileManager::PadString(CellStorage::Get3DPosAsString(cellPos), 12) + " | "
                             + FileManager::PadString(std::to_string(id), 5) + " | "
                             + FileManager::PadString(std::to_string(type), 5) + " | ");
        }
    }
}