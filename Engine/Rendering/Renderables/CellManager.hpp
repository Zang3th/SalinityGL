#pragma once

#include "Types.hpp"
#include "glm.hpp"

namespace Engine
{
    class CellManager
    {

        private:

        public:
            CellManager();
            ~CellManager();
            uint32 GetAliveCellAmount();
            void SpawnCell(CellType cellType, int32 cellAmount, glm::vec3 cellPos);
            void CalculateCellPhysics();
            void UpdateCellRenderer();
    };
}