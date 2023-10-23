#include "CellManager.hpp"

namespace Engine
{
    // ----- Private -----

    void CellManager::MoveCell(const uint32 index, const glm::u32vec3& currCellPos, const glm::u32vec3& targetCellPos)
    {
        //Move cell (set values of the target cell to the values of the current cell)
        _cellStorage.Set
        (
            {_cellStorage.Get(currCellPos).id, _cellStorage.Get(currCellPos).type},
            targetCellPos
        );

        //Delete old cell
        _cellStorage.Set({0, CellType::Air}, currCellPos);

        //Update the corresponding gpu buffers
        _cellRenderer->UpdateGPUStorage
        (
            _cellStorage.Get(targetCellPos).id,
            targetCellPos,
            CellTypeColor[_cellStorage.Get(targetCellPos).type]
        );

        //Update index
        _cellIndexStorage.at(index) = CellStorage::GetIndexFrom3DPos(targetCellPos);
    }

    bool CellManager::CellBelowIsFree(const glm::u32vec3& cellPos)
    {
        //Check if we are still in bounds
        if(cellPos.y-1 == UINT32_MAX)
            return false;

        //Get coordinates from cell below (y-1)
        glm::u32vec3 targetCellPos = glm::u32vec3(cellPos.x, cellPos.y-1, cellPos.z);

        if(_cellStorage.Get(targetCellPos).type == CellType::Air)
            return true;

        return false;
    }

    bool CellManager::GetRandomNextFreeCell(const glm::u32vec3& currCellPos, const int32 level, glm::u32vec3* targetCellPos)
    {
        //Check if we are still in bounds
        if(_cellStorage.Get(glm::u32vec3(currCellPos.x, currCellPos.y-1, currCellPos.z)).id == -1)
            return false;

        std::vector<glm::u32vec3> posToCheck;
        posToCheck.reserve(8);

        //Init vector with all positions that need to be checked
        CellStorage::GetPositionsToCheck(currCellPos, level, &posToCheck);

        //Shuffle all the entries
        Random::Shuffle(posToCheck.begin(), posToCheck.end());

        //Check vector for a free cell
        for(auto pos : posToCheck)
        {
            if(_cellStorage.Get(pos).type == CellType::Air)
            {
                *targetCellPos = pos;
                return true;
            }
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
        if(_cellStorage.Get(cellParams.pos).type == CellType::Air)
        {
            //Save cell in 3D array
            _cellStorage.Set({cellCount, cellParams.type}, cellParams.pos);

            //Save the index of the cell (how to directly access it in the 3d array)
            _cellIndexStorage.at(cellCount) = CellStorage::GetIndexFrom3DPos(cellParams.pos);

            //Update the corresponding gpu buffers
            _cellRenderer->UpdateGPUStorage
            (
                cellCount,
                glm::vec3(cellParams.pos),
                CellTypeColor[cellParams.type]
            );

            CellSimParams::cellsAlive++;
        }
    }

    void CellManager::AddCellWithoutRender(const CellParams& cellParams)
    {
        //Save cell in 3D array
        _cellStorage.Set({-1, cellParams.type}, cellParams.pos);
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
        //Cell target in which we move the current cell into
        glm::u32vec3 targetCellPos;

        //Iterate over all cell indexes
        for(uint32 i = 0; i < CellSimParams::cellsAlive; i++)
        {
            //Get coordinates from current cell
            glm::u32vec3 cellPos = CellStorage::Get3DPosFromIndex(_cellIndexStorage.at(i));

            //Check if cell below is free
            if(CellBelowIsFree(cellPos))
            {
                targetCellPos = {cellPos.x, cellPos.y-1, cellPos.z};
                MoveCell(i, cellPos, targetCellPos);
            }
            //Check all adjacent cells one level below for free space
            else if(GetRandomNextFreeCell(cellPos, -1, &targetCellPos))
            {
                MoveCell(i, cellPos, targetCellPos);
            }
            //Check all adjacent cells at the same level for free space
            else if(GetRandomNextFreeCell(cellPos, 0, &targetCellPos))
            {
                MoveCell(i, cellPos, targetCellPos);
            }
            else
            {
                //Do nothing (stay put)
            }
        }

        //Iterate over all cells

            //If current cell is water

                //Solve water physics

            //Else

                //Do nothing (stay put)
    }

    void CellManager::ResolveCellSpawners()
    {
        for(CellParams cellParams : _cellSpawnerStorage)
        {
            AddCell(cellParams);
        }
    }

    void CellManager::CreateCellWorld()
    {
        //Create ground
        for(uint32 x = 0; x < CellSimParams::CELL_FRAME_SIZE; x++)
            for(uint32 z = 0; z < CellSimParams::CELL_FRAME_SIZE; z++)
                AddCellWithoutRender({CellType::Solid, glm::u32vec3(x, 0, z)});

        //Create back wall
        for(uint32 z = 0; z < CellSimParams::CELL_FRAME_SIZE; z++)
            for(uint32 y = 0; y < CellSimParams::CELL_FRAME_SIZE; y++)
                AddCellWithoutRender({CellType::Solid, glm::u32vec3(CellSimParams::CELL_FRAME_SIZE - 1, y, z)});

        //Create side wall left
        for(uint32 x = 0; x < CellSimParams::CELL_FRAME_SIZE; x++)
            for(uint32 y = 0; y < CellSimParams::CELL_FRAME_SIZE; y++)
                AddCellWithoutRender({CellType::Solid, glm::u32vec3(x, y, 0)});

        //Create side wall right
        for(uint32 x = 0; x < CellSimParams::CELL_FRAME_SIZE; x++)
            for(uint32 y = 0; y < CellSimParams::CELL_FRAME_SIZE; y++)
                AddCellWithoutRender({CellType::Solid, glm::u32vec3(x, y, CellSimParams::CELL_FRAME_SIZE - 1)});

        //Create front wall
        for(uint32 z = 0; z < CellSimParams::CELL_FRAME_SIZE; z++)
            for(uint32 y = 0; y < CellSimParams::CELL_FRAME_SIZE; y++)
                AddCellWithoutRender({CellType::Solid, glm::u32vec3(0, y, z)});

        //Create ceiling
        for(uint32 x = 0; x < CellSimParams::CELL_FRAME_SIZE; x++)
            for(uint32 z = 0; z < CellSimParams::CELL_FRAME_SIZE; z++)
                AddCellWithoutRender({CellType::Solid, glm::u32vec3(x, CellSimParams::CELL_FRAME_SIZE - 1, z)});
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