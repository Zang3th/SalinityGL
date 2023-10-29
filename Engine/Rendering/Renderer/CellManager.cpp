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

    void CellManager::DeleteCell(const uint32 index, const glm::u32vec3& cellPos)
    {
        //ToDo: Fix
        //ToDo: Implement specific kill button in the UI

        /*_cellStorage.Set({CellTypeSpreadFactor[CellType::Air], CellType::Air}, cellPos);

        //Update the corresponding gpu buffers
        _cellRenderer->UpdateGPUStorage
        (
            _cellStorage.Get(cellPos).id,
            cellPos,
            CellTypeColor[_cellStorage.Get(cellPos).type]
        );

        //Update index
        _cellIndexStorage.at(index) = CellStorage::GetIndexFrom3DPos(cellPos);

        CellSimParams::cellsAlive--;*/
    }

    void CellManager::Fill8Neighbours(const glm::u32vec3& cellPos, std::vector<glm::u32vec3>* occupiedPos)
    {
        // ############################################ //
        //  (x+1, y, z-1) | (x+1, y, z) | (x+1, y, z+1) //
        // ---------------|-------------|---------------//
        //  (x, y, z-1)   |   cellPos   | (x, y, z+1)   //
        // ---------------|-------------|-------------- //
        //  (x-1, y, z-1) | (x-1, y, z) | (x-1, y, z+1) //
        // ############################################ //


        std::array<glm::u32vec3, 8> posToCheck
        {
            glm::u32vec3{cellPos.x+1, cellPos.y, cellPos.z-1},
            glm::u32vec3{cellPos.x+1, cellPos.y, cellPos.z},
            glm::u32vec3{cellPos.x+1, cellPos.y, cellPos.z+1},
            glm::u32vec3{cellPos.x, cellPos.y, cellPos.z-1},
            glm::u32vec3{cellPos.x, cellPos.y, cellPos.z+1},
            glm::u32vec3{cellPos.x-1, cellPos.y, cellPos.z-1},
            glm::u32vec3{cellPos.x-1, cellPos.y, cellPos.z},
            glm::u32vec3{cellPos.x-1, cellPos.y, cellPos.z+1}
        };

        //Check positions for free space and insert cell, else save occupied position
        for(auto pos : posToCheck)
        {
            if(_cellStorage.Get(pos).type == CellType::Air)
            {
                AddCell({{0, _cellStorage.Get(cellPos).type}, pos});
            }
            else
            {
                occupiedPos->push_back(pos);
            }
        }
    }

    void CellManager::MoveCellIntoDirection(const uint32 index, const glm::u32vec3& cellPos, const glm::u32vec3& originCellPos)
    {
        //int32 moveFactor = Random::GetInt32_Pseudo(2);
        int32 moveFactor = 1;
        /*if(moveFactor == 0)
        {
            return;
        }*/

        //Determine direction in which to move
        glm::i64vec3 cellDiff = glm::i64vec3(cellPos) - glm::i64vec3(originCellPos);
        cellDiff *= moveFactor;

        //Determine the cell in which to move
        glm::u32vec3 newCellPos = glm::i64vec3(cellPos) + cellDiff;

        //Try to move cell into new position
        if(_cellStorage.Get(newCellPos).type == CellType::Air)
        {
            MoveCell(index, cellPos, newCellPos);

            //Refill old cell
            AddCell({{0, CellType::Water}, cellPos});
        }
        else
        {
            _excessiveCells++;
        }
    }

    void CellManager::HandleWaterCell(const uint32 index, const glm::u32vec3& cellPos)
    {
        //Get coordinates from cell below (y-1)
        glm::u32vec3 cellPosBelow = glm::u32vec3(cellPos.x, cellPos.y-1, cellPos.z);

        glm::u32vec3 cellNeighboursToFill = cellPos;
        bool deleteCell = false;

        //Check for type of cell below
        if(_cellStorage.Get(cellPosBelow).type == CellType::Air)
        {
            MoveCell(index, cellPos, cellPosBelow);
            return;
        }
        else if(_cellStorage.Get(cellPosBelow).type == CellType::Water)
        {
            //If it's water, enable cell displacement for the cell below
            cellNeighboursToFill = cellPosBelow;

            deleteCell = true;
        }

        //Vector that gets filled with all positions that were already full
        std::vector<glm::u32vec3> occupiedBeforehand;
        occupiedBeforehand.reserve(8);
        //ToDo: Refactor vector as class variable

        //Check if spread factor allows for further distribution
        if(_cellStorage.Get(cellPos).spreadFactor > 0)
        {
            Fill8Neighbours(cellNeighboursToFill, &occupiedBeforehand);
            _cellStorage.GetModifiable(cellPos).spreadFactor = 0;
        }

        //Check the amount of cells that couldn't be filled up
        if(occupiedBeforehand.size() == 8)
        {
            //Every position was full / nothing was filled up
            //ToDo: Find out what to do now ...
            return;
        }
        //Handle the non-fillable cells
        else if(!occupiedBeforehand.empty())
        {
            for(auto pos : occupiedBeforehand)
            {
                if(_cellStorage.Get(pos).type == CellType::Water)
                {
                    MoveCellIntoDirection(index, pos, cellNeighboursToFill);
                }
            }
            //ToDo: Clear vector
        }

        //Clean up
        _cellStorage.GetModifiable(cellPos).movedDownLastTurn = false;

        if(deleteCell)
        {
            //Delete cell and increment excessive cell counter
            DeleteCell(index, cellPos);
            _excessiveCells++;
        }
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
        _excessiveCells = 0;
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
        Logger::Print("Count | 3D-Index | (X, Y, Z)    | ID    | Type  | Spread | MoveLT |");

        for(uint32 i = 0; i < CellSimParams::cellsAlive; i++)
        {
            uint32 index = _cellIndexStorage.at(i);
            glm::u32vec3 cellPos = CellStorage::Get3DPosFromIndex(_cellIndexStorage.at(i));
            Cell cell = _cellStorage.Get(cellPos);

            Logger::Print(FileManager::PadString(std::to_string(i), 5) + " | "
                          + FileManager::PadString(std::to_string(index), 8) + " | "
                          + FileManager::PadString(CellStorage::Get3DPosAsString(cellPos), 12) + " | "
                          + FileManager::PadString(std::to_string(cell.id), 5) + " | "
                          + FileManager::PadString(std::to_string(cell.type), 5) + " | "
                          + FileManager::PadString(std::to_string(cell.spreadFactor), 6) + " | "
                          + FileManager::PadString(std::to_string(cell.movedDownLastTurn), 6) + " | ");
        }

        Logger::Print("Excessive cell counter: " + std::to_string(_excessiveCells));
    }
}