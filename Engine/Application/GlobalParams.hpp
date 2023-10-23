#pragma once

#include "Types.hpp"
#include "glm.hpp"

namespace Engine
{
    // #######################################################################################################
    // ############################################### General ###############################################
    // #######################################################################################################

    struct WindowParams
    {
        inline static constexpr int32 WIDTH              = 1920;
        inline static constexpr int32 HEIGHT             = 1080;
        inline static           bool  wireframeRendering = false;
        inline static           bool  debugSprites       = false;
        inline static           bool  resetCamera        = false;

        WindowParams() = delete;
    };

    struct RenderParams
    {
        inline static constexpr float  GRAVITY   = -20.0f;
        inline static           float  nearPlane = 1.0f;
        inline static           float  farPlane  = 512.0f;
        inline static           uint32 planeSize = 128;

        RenderParams() = delete;
    };

    struct CameraParams
    {
        inline static float     movementSpeed = 25.0f;
        inline static float     startYaw      = 0.0f;
        inline static float     startPitch    = 0.0f;
        inline static glm::vec3 startPos      = glm::vec3(-50.0f, 0.0f, 0.0f);

        CameraParams() = delete;
    };

    struct LightParams
    {
        inline static glm::vec3 position = glm::vec3(565.0f, 170.0f, 455.0f);
        inline static glm::vec3 target   = glm::vec3(515.0f, 40.0f, 505.0f);
        inline static glm::vec3 color    = glm::vec3(1.0f, 1.0f, 1.0f);

        LightParams() = delete;
    };

    struct RenderStatistics
    {
        inline static uint32 drawCalls      = 0;
        inline static uint32 drawnVertices  = 0;
        inline static uint32 cubemapPasses  = 0;
        inline static uint32 terrainPasses  = 0;
        inline static uint32 modelPasses    = 0;
        inline static uint32 spritePasses   = 0;
        inline static uint32 waterPasses    = 0;
        inline static uint32 particlePasses = 0;
        inline static uint32 cellPasses     = 0;

        RenderStatistics() = delete;

        static void Reset()
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

    // #######################################################################################################
    // ############################################### GreenWorld ############################################
    // #######################################################################################################

    //Nothing here

    // #######################################################################################################
    // ############################################### CellSim ###############################################
    // #######################################################################################################

    //Read in of cell types via X-Macro
    enum CellType
    {
    #   define X(a, b, c) a,
    #   include "CellTypes.def"
    #   undef X
    };

    inline static const char* const CellTypeStrings[] =
    {
    #   define X(a, b, c) #a,
    #   include "CellTypes.def"
    #   undef X
    };

    inline static const float CellTypeMass[] =
    {
    #   define X(a, b, c) b,
    #   include "CellTypes.def"
    #   undef X
    };

    inline static const glm::vec3 CellTypeColor[] =
    {
    #   define X(a, b, c) c,
    #   include "CellTypes.def"
    #   undef X
    };

    struct CellSimParams
    {
        inline static constexpr uint32   CELL_FRAME_SIZE       = 61;
        inline static constexpr uint32   MAX_CELL_AMOUNT       = CELL_FRAME_SIZE * CELL_FRAME_SIZE * CELL_FRAME_SIZE;
        inline static           CellType selectedCellType      = Water;
        inline static           uint32   selectedCellCoords[3] = {0, 0, 0};
        inline static           uint32   selectedCellAmount    = 0;
        inline static           uint32   cellsAlive            = 0;
        inline static           bool     spawnNewCell          = false;
        inline static           bool     createSpawner         = false;
        inline static           bool     deleteCells           = false;
        inline static           bool     deleteSpawners        = false;
        inline static           bool     printDebug            = false;
    };
}