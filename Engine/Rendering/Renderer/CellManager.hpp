#include "CellStorage.hpp"
#include "Types.hpp"
#include "GlobalParams.hpp"
#include "RenderManager.hpp"

namespace Engine
{
    class CellManager
    {
        private:
            CellRenderer*                                      _cellRenderer = nullptr;
            CellStorage                                        _cellStorage;
            std::array<uint32, CellSimParams::MAX_CELL_AMOUNT> _cellIndexStorage = {0};

            void MoveCellDown(uint32 index, const glm::u32vec3& cellPos, const glm::u32vec3& cellPosBelow);

        public:
            CellManager() = default;
            void AddCellRenderer(float cellSize, const std::string& shader, const glm::vec3& worldSpawnPos);
            void SpawnCell(CellType cellType, const glm::u32vec3& cellPos);
            void DeleteAllCells();
            void CalculateCellPhysics();
            void PrintDebug();
    };
}