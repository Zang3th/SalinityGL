#include "CellStorage.hpp"

namespace Engine
{
    CellStorage::CellStorage()
    {
        Init();
    }

    void CellStorage::Init()
    {
        for(uint32 x = 0; x < CellSimParams::CELL_FRAME_SIZE; x++)
        {
            for(uint32 y = 0; y < CellSimParams::CELL_FRAME_SIZE; y++)
            {
                for(uint32 z = 0; z < CellSimParams::CELL_FRAME_SIZE; z++)
                {
                    _cellBuffer[x][y][z] = {0, CellType::None};
                }
            }
        }
    }

    Cell CellStorage::Get(const glm::u32vec3& cellPos)
    {
        return _cellBuffer[cellPos.x][cellPos.y][cellPos.z];
    }

    void CellStorage::Set(const CellParams& cellParams)
    {
        _cellBuffer[cellParams.pos.x][cellParams.pos.y][cellParams.pos.z] = cellParams.cell;
    }

    uint32 CellStorage::GetIndexFrom3DPos(const glm::u32vec3& cellPos)
    {
        return (cellPos.x * CellSimParams::CELL_FRAME_SIZE * CellSimParams::CELL_FRAME_SIZE )
             + (cellPos.y * CellSimParams::CELL_FRAME_SIZE ) + cellPos.z;
    }

    glm::vec3 CellStorage::Get3DPosFromIndex(uint32 index)
    {
        uint32 x = index / (CellSimParams::CELL_FRAME_SIZE * CellSimParams::CELL_FRAME_SIZE);
        index -= (x * CellSimParams::CELL_FRAME_SIZE * CellSimParams::CELL_FRAME_SIZE );
        uint32 y = index / CellSimParams::CELL_FRAME_SIZE;
        uint32 z = index % CellSimParams::CELL_FRAME_SIZE;
        return glm::u32vec3(x, y, z);
    }

    std::string CellStorage::Get3DPosAsString(const glm::u32vec3& cellPos)
    {
        return "(" + std::to_string(cellPos.x) + ", " + std::to_string(cellPos.y) + ", " + std::to_string(cellPos.z) + ")";
    }

    void CellStorage::GetPositionsToCheck(const glm::u32vec3& cellPos, const int32 level, std::vector<glm::u32vec3>* posToCheck)
    {
        // ################################################################################################## //
        // Position 1: (x+1, y+level, z-1) | Position 2: (x+1, y+level, z) | Position 3: (x+1, y+level, z+1)  //
        // --------------------------------|-------------------------------|--------------------------------- //
        // Position 4: (x,   y+level, z-1) |          currCellPos          | Position 6: (x,   y+level, z+1)  //
        // --------------------------------|-------------------------------|--------------------------------- //
        // Position 7: (x-1, y+level, z-1) | Position 8: (x-1, y+level, z) | Position 9: (x-1, y+level, z+1)  //
        // ################################################################################################## //

        //Get all boundary violations
        bool x_posInBounds = (cellPos.x+1 < CellSimParams::CELL_FRAME_SIZE);
        bool x_negInBounds = (cellPos.x-1 != UINT32_MAX);
        bool z_posInBounds = (cellPos.z+1 < CellSimParams::CELL_FRAME_SIZE);
        bool z_negInBounds = (cellPos.z-1 != UINT32_MAX);

        if(x_posInBounds)
        {
            // Position 1
            if(z_negInBounds)
                posToCheck->emplace_back(cellPos.x+1, cellPos.y+level, cellPos.z-1);

            // Position 2
            posToCheck->emplace_back(cellPos.x+1, cellPos.y+level, cellPos.z);

            // Position 3
            if(z_posInBounds)
                posToCheck->emplace_back(cellPos.x+1, cellPos.y+level, cellPos.z+1);
        }

        if(x_negInBounds)
        {
            // Position 7
            if(z_negInBounds)
            {
                posToCheck->emplace_back(cellPos.x-1, cellPos.y+level, cellPos.z-1);
            }

            // Position 8
            posToCheck->emplace_back(cellPos.x-1, cellPos.y+level, cellPos.z);
        }

        if(z_posInBounds)
        {
            // Position 6
            posToCheck->emplace_back(cellPos.x, cellPos.y+level, cellPos.z+1);

            // Position 9
            if(x_negInBounds)
                posToCheck->emplace_back(cellPos.x-1, cellPos.y+level, cellPos.z+1);
        }

        if(z_negInBounds)
        {
            // Position 4
            posToCheck->emplace_back(cellPos.x, cellPos.y+level, cellPos.z-1);
        }
    }
}