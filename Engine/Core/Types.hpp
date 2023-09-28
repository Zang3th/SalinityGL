#pragma once

#include <memory>
#include <cstdint>

namespace Engine
{
    // --- Smart-Pointer-Stuff
    template<typename T>
    using Scope = std::unique_ptr<T>;

    template<typename T, typename ... Args>
    constexpr Scope<T> MakeScope(Args&&... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using Ref = std::shared_ptr<T>;

    template<typename T, typename ... Args>
    constexpr Ref<T> MakeRef(Args&&... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    // --- Renaming stuff
    typedef int8_t   int8;
    typedef int16_t  int16;
    typedef int32_t  int32;
    typedef int64_t  int64;
    typedef uint8_t  uint8;
    typedef uint16_t uint16;
    typedef uint32_t uint32;
    typedef uint64_t uint64;

    // --- Custom datatypes
    struct RenderStats
    {
        uint32 drawCalls      = 0;
        uint32 drawnVertices  = 0;
        uint32 cubemapPasses  = 0;
        uint32 terrainPasses  = 0;
        uint32 modelPasses    = 0;
        uint32 spritePasses   = 0;
        uint32 waterPasses    = 0;
        uint32 particlePasses = 0;
        uint32 cellPasses     = 0;

        void Reset()
        {
            drawCalls      = 0;
            drawnVertices  = 0;
            cubemapPasses  = 0;
            terrainPasses  = 0;
            modelPasses    = 0;
            spritePasses   = 0;
            waterPasses    = 0;
            particlePasses = 0;
            cellPasses     = 0;
        }
    };

    // --- Read in of cell types via X-Macro
    enum CellType
    {
    #   define X(a) a,
    #   include "CellTypes.def"
    #   undef X
    };

    inline static const char* const CellTypeStrings[] =
    {
    #   define X(a) #a,
    #   include "CellTypes.def"
    #   undef X
    };

    // --- Application settings
    struct AppSettings //ToDo: Separate all application settings from another
    {
        // --- General
        inline static constexpr int32       WINDOW_WIDTH       = 1920;
        inline static constexpr int32       WINDOW_HEIGHT      = 1080;
        inline static           uint32      planeSize          = 64;
        inline static           bool        wireframeRendering = false;
        inline static           bool        debugSprites       = false;
        inline static           RenderStats renderStats;

        // --- GreenWorld
        inline static constexpr float       GRAVITY            = -20.0f;

        // --- CellSim
        inline static constexpr uint32      CELL_FRAME_SIZE       = 61;
        inline static constexpr uint32      MAX_CELL_AMOUNT       = CELL_FRAME_SIZE * CELL_FRAME_SIZE * CELL_FRAME_SIZE;
        inline static           bool        resetCamera           = false;
        inline static           CellType    selectedCellType      = None;
        inline static           int32       selectedCellCoords[3] = {0, 0, 0};
        inline static           int32       selectedCellAmount    = 0;
        inline static           uint32      cellsAlive            = 0;
        inline static           bool        spawnNewCell          = false;

        AppSettings() = delete;
    };
}