#include "Types.hpp"
#include "GlobalParams.hpp"

#include <string>
#include <vector>

namespace Engine
{
    struct Cell
    {
        uint32 id;
        CellType type;
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
            void Set(const CellParams& cellParams);
            static uint32 GetIndexFrom3DPos(const glm::u32vec3& cellPos);
            static glm::vec3 Get3DPosFromIndex(uint32 index);
            static std::string Get3DPosAsString(const glm::u32vec3& cellPos);
            static void GetPositionsToCheck(const glm::u32vec3& cellPos, int32 level, std::vector<glm::u32vec3>* posToCheck);
    };
}