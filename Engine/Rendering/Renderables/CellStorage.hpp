#pragma once

#include "Types.hpp"
#include "GlobalParams.hpp"

#include <string>

namespace Engine
{
    struct Cell
    {
        uint8 spreadFactor = 0;
        CellType type = CellType::Air;
        uint32 id = UINT32_MAX;
        bool movedLastTurn = false;
    };

    struct CellParams
    {
        Cell cell;
        glm::u32vec3 pos;
    };

    class CellStorage
    {
        private:
            Cell _cellBuffer[CellSimParams::CELL_FRAME_SIZE][CellSimParams::CELL_FRAME_SIZE][CellSimParams::CELL_FRAME_SIZE];

        public:
            CellStorage();
            ~CellStorage() = default;
            void Init();
            Cell Get(const glm::u32vec3& cellPos);
            Cell& GetModifiable(const glm::u32vec3& cellPos);
            void Set(const Cell& cell, const glm::u32vec3& cellPos);
            static uint32 GetIndexFrom3DPos(const glm::u32vec3& cellPos);
            static glm::vec3 Get3DPosFromIndex(uint32 index);
            static std::string Get3DPosAsString(const glm::u32vec3& cellPos);
    };
}