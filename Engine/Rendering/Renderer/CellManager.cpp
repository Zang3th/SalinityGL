#include "CellManager.hpp"

namespace Engine
{
    // ----- Private -----

    void CellManager::MoveCell(const uint32 index, const glm::u32vec3& currCellPos, const glm::u32vec3& targetCellPos)
    {
        //Move cell (set values of the target cell to the values of the current cell)
        _cellStorage.Set({_cellStorage.Get(currCellPos), targetCellPos});

        //Delete old cell
        _cellStorage.Set({{0, CellType::None}, currCellPos});

        //Update the corresponding model view buffer for upload to the gpu
        _cellRenderer->UpdatePositionStorage(_cellStorage.Get(targetCellPos).id, targetCellPos);

        //Update index
        _cellIndexStorage.at(index) = CellStorage::GetIndexFrom3DPos(targetCellPos);
    }

    bool CellManager::GetRandomNextFreeCell(const glm::u32vec3& currCellPos, glm::u32vec3* targetCellPos)
    {
        // #######################################################################
        // Position 1: (x+1, z-1) | Position 2: (x+1, z) | Position 3: (x+1, z+1)
        // -----------------------|----------------------|------------------------
        // Position 4: (x,   z-1) |     currCellPos      | Position 6: (x,   z+1)
        // -----------------------|----------------------|------------------------
        // Position 7: (x-1, z-1) | Position 8: (x-1, z) | Position 9: (x-1, z+1)
        // #######################################################################

        const uint32 arraySize = 8;

        //Init array with all positions that need to be checked
        std::array<glm::u32vec3, arraySize> posToCheck
        {
            glm::u32vec3(currCellPos.x+1, currCellPos.y, currCellPos.z-1), //Position 1
            glm::u32vec3(currCellPos.x+1, currCellPos.y, currCellPos.z),   //Position 2
            glm::u32vec3(currCellPos.x+1, currCellPos.y, currCellPos.z+1), //Position 3
            glm::u32vec3(currCellPos.x,   currCellPos.y, currCellPos.z-1), //Position 4
                                                                           //currCellPos
            glm::u32vec3(currCellPos.x,   currCellPos.y, currCellPos.z+1),  //Position 6
            glm::u32vec3(currCellPos.x-1, currCellPos.y, currCellPos.z-1),  //Position 7
            glm::u32vec3(currCellPos.x-1, currCellPos.y, currCellPos.z),    //Position 8
            glm::u32vec3(currCellPos.x-1, currCellPos.y, currCellPos.z+1)   //Position 9
        };

        //Shuffle all entries
        Random::Shuffle(posToCheck.begin(), posToCheck.end());

        uint32 index = 0;

        //Check until a free cell is found or end of array is reached
        while(index < arraySize)
        {
            //ToDo: Fix this
            if(_cellStorage.Get(posToCheck[index]).type == CellType::None)
            {
                *targetCellPos = posToCheck[index];
                return true;
            }

            index++;
        }

        return false;
    }

    // ----- Public -----

    CellManager::CellManager()
    {
        _cellSpawnerStorage.reserve(5);
    }

    void CellManager::AddCellRenderer(const std::string& shader, const glm::vec3& worldSpawnPos)
    {
        if(RenderManager::GetInitStatus())
        {
            _cellRenderer = Engine::RenderManager::AddCellRenderer(shader, worldSpawnPos);
        }
        else
        {
            Logger::Error("Failed", "CellRenderer", "RenderManager wasn't initialized!");
        }
    }

    void CellManager::AddCell(const CellParams& cellParams)
    {
        uint32 cellCount = CellSimParams::cellsAlive;

        //Check if cell is empty
        if(_cellStorage.Get(cellParams.pos).type == CellType::None)
        {
            //Save cell in 3D array with type and amount
            _cellStorage.Set({{cellCount, cellParams.cell.type}, cellParams.pos});

            //Save the index of the cell (how to directly access it in the 3d array)
            _cellIndexStorage.at(cellCount) = CellStorage::GetIndexFrom3DPos(cellParams.pos);

            //Update the corresponding model view buffer for upload to the gpu
            _cellRenderer->UpdatePositionStorage(cellCount, glm::vec3(cellParams.pos));

            CellSimParams::cellsAlive++;
        }
    }

    void CellManager::AddCellSpawner(const CellParams& cellParams)
    {
        _cellSpawnerStorage.push_back(cellParams);
    }

    void CellManager::DeleteCells()
    {
        _cellStorage.Init();
        CellSimParams::cellsAlive = 0;
    }

    void CellManager::DeleteSpawners()
    {
        _cellSpawnerStorage.clear();
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
                //Set to coordinates from cell below (y - 1)
                glm::u32vec3 targetCellPos = glm::u32vec3(cellPos.x, cellPos.y - 1, cellPos.z);

                //Check if cell below is free
                if(_cellStorage.Get(targetCellPos).type == CellType::None)
                {
                    MoveCell(i, cellPos, targetCellPos);
                }
                //Check all cells one level below for free space (adjacent cells first)
                else
                {
                    //If a free cell was found
                    if(GetRandomNextFreeCell(cellPos, &targetCellPos))
                    {
                        MoveCell(i, cellPos, targetCellPos);
                    }
                }
            }
        }
    }

    void CellManager::ResolveCellSpawners()
    {
        for(CellParams cellParams : _cellSpawnerStorage)
        {
            AddCell(cellParams);
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