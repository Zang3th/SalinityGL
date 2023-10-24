#include "CellManager.hpp"

namespace Engine
{
    // ----- Private -----

    void CellManager::MoveCell(const uint32 index, const glm::u32vec3& cellPos, const glm::u32vec3& targetCellPos)
    {
        //Move cell (set values of the target cell to the values of the current cell)
        _cellStorage.Set(_cellStorage.Get(cellPos), targetCellPos);

        //Modify movement state
        _cellStorage.GetModifiable(targetCellPos).movedDownLastTurn = true;

        //Delete old cell
        _cellStorage.Set({CellTypeSpreadFactor[CellType::Air], CellType::Air}, cellPos);

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

    void CellManager::FillFree4Neighbours(const glm::u32vec3& cellPos)
    {
        // ############################################################ //
        //                    | Front: (x+1, y, z) |                    //
        // ----------------------------------------|------------------- //
        //  Left: (x, y, z-1) |      cellPos       | Right: (x, y, z+1) //
        // ----------------------------------------|------------------- //
        //                    | Back: (x-1, y, z)  |                    //
        // ############################################################ //

        std::array<glm::u32vec3, 4> posToCheck
        {
            glm::u32vec3{cellPos.x+1, cellPos.y, cellPos.z}, //Front
            glm::u32vec3{cellPos.x, cellPos.y, cellPos.z-1}, //Left
            glm::u32vec3{cellPos.x, cellPos.y, cellPos.z+1}, //Right
            glm::u32vec3{cellPos.x-1, cellPos.y, cellPos.z}  //Back
        };

        //Check 4-Neighbours for free space
        for(auto pos : posToCheck)
        {
            if(_cellStorage.Get(pos).type == CellType::Air)
            {
                AddCell({{0, _cellStorage.Get(cellPos).type}, pos});
            }
        }
    }

    void CellManager::DisplaceCellsBelow(const glm::u32vec3& cellPos)
    {
        //ToDo: Implement

        Logger::Print("Displace cells below " + CellStorage::Get3DPosAsString(cellPos));
    }

    void CellManager::HandleWaterCell(const uint32 index, const glm::u32vec3& cellPos)
    {
        //Get coordinates from cell below (y-1)
        glm::u32vec3 cellPosBelow = glm::u32vec3(cellPos.x, cellPos.y-1, cellPos.z);

        //Check cell below for free space
        if(_cellStorage.Get(cellPosBelow).type == CellType::Air)
        {
            MoveCell(index, cellPos, cellPosBelow);
            return;
        }

        //Check if spread factor allows further distribution
        if(_cellStorage.Get(cellPos).spreadFactor > 0)
        {
            FillFree4Neighbours(cellPos);
        }

        //Check if cell moved down last turn
        if(_cellStorage.Get(cellPos).movedDownLastTurn)
        {
            //If so, displace cells below
            DisplaceCellsBelow(cellPos);
        }

        _cellStorage.GetModifiable(cellPos).movedDownLastTurn = false;
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
            //Modify cell parameters
            CellParams cellParamsCopy = cellParams;
            cellParamsCopy.cell.id = cellCount;

            //Save cell in 3D array
            _cellStorage.Set(cellParamsCopy.cell, cellParamsCopy.pos);

            //Save the index of the cell (how to directly access it in the 3d array)
            _cellIndexStorage.at(cellCount) = CellStorage::GetIndexFrom3DPos(cellParamsCopy.pos);

            //Update the corresponding gpu buffers
            _cellRenderer->UpdateGPUStorage
            (
                cellCount,
                glm::vec3(cellParamsCopy.pos),
                CellTypeColor[cellParamsCopy.cell.type]
            );

            CellSimParams::cellsAlive++;
        }
    }

    void CellManager::AddCellWithoutRender(const CellParams& cellParams)
    {
        //Save cell in 3D array
        _cellStorage.Set(cellParams.cell, cellParams.pos);
    }

    void CellManager::AddCellSpawner(const CellParams& cellParams)
    {
        _cellSpawnerStorage.push_back(cellParams);
    }

    void CellManager::DeleteCells()
    {
        _cellStorage.Init();
        CreateCellWorld();
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

            //If current cell is water
            if(_cellStorage.Get(cellPos).type == CellType::Water)
            {
                //Solve water physics
                HandleWaterCell(i, cellPos);
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

    void CellManager::CreateCellWorld()
    {
        //Create cell to spawn
        Cell cell{CellTypeSpreadFactor[CellType::Solid], CellType::Solid};

        //Create ground
        for(uint32 x = 0; x < CellSimParams::CELL_FRAME_SIZE; x++)
            for(uint32 z = 0; z < CellSimParams::CELL_FRAME_SIZE; z++)
                AddCellWithoutRender({cell, glm::u32vec3(x, 0, z)});

        //Create back wall
        for(uint32 z = 0; z < CellSimParams::CELL_FRAME_SIZE; z++)
            for(uint32 y = 0; y < CellSimParams::CELL_FRAME_SIZE; y++)
                AddCellWithoutRender({cell, glm::u32vec3(CellSimParams::CELL_FRAME_SIZE - 1, y, z)});

        //Create side wall left
        for(uint32 x = 0; x < CellSimParams::CELL_FRAME_SIZE; x++)
            for(uint32 y = 0; y < CellSimParams::CELL_FRAME_SIZE; y++)
                AddCellWithoutRender({cell, glm::u32vec3(x, y, 0)});

        //Create side wall right
        for(uint32 x = 0; x < CellSimParams::CELL_FRAME_SIZE; x++)
            for(uint32 y = 0; y < CellSimParams::CELL_FRAME_SIZE; y++)
                AddCellWithoutRender({cell, glm::u32vec3(x, y, CellSimParams::CELL_FRAME_SIZE - 1)});

        //Create front wall
        for(uint32 z = 0; z < CellSimParams::CELL_FRAME_SIZE; z++)
            for(uint32 y = 0; y < CellSimParams::CELL_FRAME_SIZE; y++)
                AddCellWithoutRender({cell, glm::u32vec3(0, y, z)});

        //Create ceiling
        for(uint32 x = 0; x < CellSimParams::CELL_FRAME_SIZE; x++)
            for(uint32 z = 0; z < CellSimParams::CELL_FRAME_SIZE; z++)
                AddCellWithoutRender({cell, glm::u32vec3(x, CellSimParams::CELL_FRAME_SIZE - 1, z)});
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