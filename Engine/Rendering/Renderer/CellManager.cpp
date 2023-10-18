#include "CellManager.hpp"

namespace Engine
{
    // ----- Private -----

    void CellManager::MoveCellDown(const uint32 index, const glm::u32vec3& cellPos, const glm::u32vec3& cellPosBelow)
    {
        //Move cell down (set values of the cell below to the values of the current cell)
        _cellStorage.Set(cellPosBelow, _cellStorage.Get(cellPos));

        //Delete old cell
        _cellStorage.Set(cellPos, {0, CellType::None});

        //Update the corresponding model view buffer for upload to the gpu
        _cellRenderer->UpdateModelViewStorage(_cellStorage.Get(cellPosBelow).id, cellPosBelow);

        //Update index
        _cellIndexStorage.at(index) = CellStorage::GetIndexFrom3DPos(cellPosBelow);
    }

    // ----- Public -----

    void CellManager::AddCellRenderer(float cellSize, const std::string& shader, const glm::vec3& worldSpawnPos)
    {
        if(RenderManager::GetInitStatus())
        {
            _cellRenderer = Engine::RenderManager::AddCellRenderer(cellSize, shader, worldSpawnPos);
        }
        else
        {
            Logger::Warn("Adding", "CellRenderer", "RenderManager wasn't initialized!");
        }
    }

    void CellManager::SpawnCell(CellType cellType, const glm::u32vec3& cellPos)
    {
        uint32 cellCount = CellSimParams::cellsAlive;

        //Check if cell is empty
        if(_cellStorage.Get(cellPos).type == CellType::None)
        {
            //Save cell in 3D array with type and amount
            _cellStorage.Set(cellPos, {cellCount, cellType});

            //Save the index of the cell (how to directly access it in the 3d array)
            _cellIndexStorage.at(cellCount) = CellStorage::GetIndexFrom3DPos(cellPos);

            //Update the corresponding model view buffer for upload to the gpu
            _cellRenderer->UpdateModelViewStorage(cellCount, glm::vec3((float)cellPos.x, (float)cellPos.y, (float)cellPos.z));

            CellSimParams::cellsAlive++;
        }
    }

    void CellManager::DeleteAllCells()
    {
        _cellStorage.Init();
        CellSimParams::cellsAlive = 0;
    }

    void CellManager::CalculateCellPhysics()
    {
        //Iterate over all cell indexes
        for(uint32 i = 0; i < CellSimParams::cellsAlive; i++)
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

    void CellManager::PrintDebug()
    {
        Logger::LineBreak();
        Logger::Print("Count | 3D-Index | (X, Y, Z)    | ID    | Type  |");

        for(uint32 i = 0; i < CellSimParams::cellsAlive; i++)
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