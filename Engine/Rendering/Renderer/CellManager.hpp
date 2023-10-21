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
            std::vector<CellParams>                            _cellSpawnerStorage;

            void MoveCellDown(uint32 index, const glm::u32vec3& cellPos, const glm::u32vec3& cellPosBelow);

        public:
            CellManager();
            void AddCellRenderer(float cellSize, const std::string& shader, const glm::vec3& worldSpawnPos);
            void AddCell(const CellParams& cellParams);
            void AddCellSpawner(const CellParams& cellParams);
            void DeleteCells();
            void DeleteSpawners();
            void CalculateCellPhysics();
            void ResolveCellSpawners();
            void PrintDebug();
    };
}