#include "CellRenderer.hpp"

namespace Engine
{
    // ----- Private -----

    CellRenderer::CellRenderer(float cellSize, Shader* shader, const glm::vec3& worldSpawnPos)
        :   _cellSize(cellSize), _verticeCount(36), _cellCount(0), _shader(shader), _worldSpawnPos(worldSpawnPos)
    {
        Logger::Info("Created", "Renderer",__func__);
        InitCellStorage();
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

    void CellRenderer::UpdateModelViewStorage(uint32 index, const glm::vec3& pos)
    {
        glm::mat4 model(1.0f);
        model = glm::translate(model, _worldSpawnPos + pos);
        model = glm::scale(model, glm::vec3(_cellSize));
        _modelViewStorage.at(index) = model;
    }

    void CellRenderer::InitCellStorage()
    {
        for(uint32 x = 0; x < CellSimParams::CELL_FRAME_SIZE; x++)
        {
            for(uint32 y = 0; y < CellSimParams::CELL_FRAME_SIZE; y++)
            {
                for(uint32 z = 0; z < CellSimParams::CELL_FRAME_SIZE; z++)
                {
                    _cellStorage[x][y][z] = {0, 0, CellType::None};
                }
            }
        }
    }

    inline uint32 CellRenderer::GetIndexFromCoords(const glm::u32vec3& cellPos)
    {
        return (cellPos.x * CellSimParams::CELL_FRAME_SIZE * CellSimParams::CELL_FRAME_SIZE ) + (cellPos.y * CellSimParams::CELL_FRAME_SIZE ) + cellPos.z;
    }

    inline glm::vec3 CellRenderer::GetCoordsFromIndex(uint32 index)
    {
        uint32 x = index / (CellSimParams::CELL_FRAME_SIZE * CellSimParams::CELL_FRAME_SIZE);
        index -= (x * CellSimParams::CELL_FRAME_SIZE * CellSimParams::CELL_FRAME_SIZE );
        uint32 y = index / CellSimParams::CELL_FRAME_SIZE;
        uint32 z = index % CellSimParams::CELL_FRAME_SIZE;
        return glm::u32vec3(x, y, z);
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

    void CellRenderer::SpawnCell(CellType cellType, uint32 cellAmount, const glm::u32vec3& cellPos)
    {
        //ToDo: Find a better way to solve this
        if(cellType != CellType::None && cellAmount > 0)
        {
            //Save cell in 3D array with type and amount
            _cellStorage[cellPos.x][cellPos.y][cellPos.z] = {_cellCount, cellAmount, cellType};

            //Save the index of the cell (how to directly access it in the 3d array)
            _cellIndexStorage.at(_cellCount) = GetIndexFromCoords(cellPos);

            //Update the corresponding model view buffer for upload to the gpu
            UpdateModelViewStorage(_cellCount, glm::vec3((float)cellPos.x, (float)cellPos.y, (float)cellPos.z));

            _cellCount++;
        }
    }

    void CellRenderer::DeleteAllCells()
    {
        _cellCount = 0;
    }

    void CellRenderer::CalculateCellPhysics()
    {
        //ToDo: Fix this bullshit
        //Iterate over all cell indexes
        for(uint32 i = 0; i < _cellCount; i++)
        {
            //Recalculate the corresponding 3D coordinates
            glm::u32vec3 cellCoords = GetCoordsFromIndex(_cellIndexStorage.at(i));

            //Check if the cell is not touching the ground
            if(cellCoords.y > 0)
            {
                //Check if there is a free cell below
                if(_cellStorage[cellCoords.x][cellCoords.y - 1][cellCoords.z].amount == 0)
                {
                    //Move cell down
                    _cellStorage[cellCoords.x][cellCoords.y - 1][cellCoords.z].type   = _cellStorage[cellCoords.x][cellCoords.y][cellCoords.z].type;
                    _cellStorage[cellCoords.x][cellCoords.y - 1][cellCoords.z].amount = _cellStorage[cellCoords.x][cellCoords.y][cellCoords.z].amount;

                    //Delete old cell
                    _cellStorage[cellCoords.x][cellCoords.y][cellCoords.z].type = CellType::None;
                    _cellStorage[cellCoords.x][cellCoords.y][cellCoords.z].amount = 0;

                    //Update the corresponding model view buffer for upload to the gpu
                    UpdateModelViewStorage(_cellStorage[cellCoords.x][cellCoords.y - 1][cellCoords.z].id, glm::vec3((float)cellCoords.x, (float)cellCoords.y - 1, (float)cellCoords.z));

                    //Update index
                    _cellIndexStorage.at(i) = GetIndexFromCoords(glm::u32vec3(cellCoords.x, cellCoords.y - 1, cellCoords.z));
                }
            }
        }

        //ToDo: Resolve amount > 1 in one cell
    }
}