#include "CellStorage.hpp"
#include "Types.hpp"
#include "GlobalParams.hpp"
#include "RenderManager.hpp"
#include "Random.hpp"

namespace Engine
{
    class CellManager
    {
        private:
            CellRenderer*                                      _cellRenderer = nullptr;
            CellStorage                                        _cellStorage;
            std::array<uint32, CellSimParams::MAX_CELL_AMOUNT> _cellIndexStorage = {0};
            std::vector<CellParams>                            _cellSpawnerStorage;

            void MoveCell(uint32 index, const glm::u32vec3& cellPos, const glm::u32vec3& targetCellPos);
            void FillFree4Neighbours(const glm::u32vec3& cellPos);
            void DisplaceCellsBelow(const glm::u32vec3& cellPos);
            void HandleWaterCell(uint32 index, const glm::u32vec3& cellPos);

        public:
            CellManager();
            void AddCellRenderer(const std::string& shader, const glm::vec3& worldSpawnPos);
            void AddCell(const CellParams& cellParams);
            void AddCellWithoutRender(const CellParams& cellParams);
            void AddCellSpawner(const CellParams& cellParams);
            void DeleteCells();
            void DeleteSpawners();
            void CalculateCellPhysics();
            void ResolveCellSpawners();
            void CreateCellWorld();
            void PrintDebug();
    };
}