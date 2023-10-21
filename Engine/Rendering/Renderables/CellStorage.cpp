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
}