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
        inline static constexpr int32 WIDTH  = 1920;
        inline static constexpr int32 HEIGHT = 1080;

        WindowParams() = delete;
    };

    struct RenderParams
    {
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

    struct UIParams
    {
        inline static bool showOverlay        = true;
        inline static bool wireframeRendering = false;
        inline static bool debugSprites       = false;
        inline static bool resetCamera        = false;

        UIParams() = delete;
    };

    struct PhysicsParams
    {
        inline static constexpr float  GRAVITY   = -9.81f;

        PhysicsParams() = delete;
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

    inline static const uint8 CellTypeSpreadFactor[] =
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
        inline static constexpr uint32   MAX_RECURSION_DEPTH   = CELL_FRAME_SIZE;
        inline static           CellType selectedCellType      = Water;
        inline static           uint32   selectedCellCoords[3] = {0, 0, 0};
        inline static           uint32   selectedCellAmount    = 0;
        inline static           uint32   cellsAlive            = 0;
        inline static           bool     spawnCell             = false;
        inline static           bool     killCell              = false;
        inline static           bool     createSpawner         = false;
        inline static           bool     deleteCells           = false;
        inline static           bool     deleteSpawners        = false;
        inline static           bool     printDebug            = false;
        inline static           bool     enableSingleStepping  = false;
        inline static           bool     performSingleStep     = false;

        CellSimParams() = delete;
    };

    // #######################################################################################################
    // ############################################### Liquefied #############################################
    // #######################################################################################################

    enum Integrator
    {
        ForwardEuler         = 0,
        RungeKutta2          = 1,
        RungeKutta3          = 2,
    };

    enum Visualization
    {
        Greyscale = 0,
        BlackBody = 1,
        ParaView  = 2,
    };

    struct LiquefiedParams
    {
        inline static constexpr uint32        SIMULATION_WIDTH            = 150;
        inline static constexpr uint32        SIMULATION_HEIGHT           = 100;
        inline static constexpr uint32        LIQUID_NUM_CELLS            = SIMULATION_WIDTH * SIMULATION_HEIGHT;
        inline static constexpr uint32        GAUSS_SEIDEL_ITERATIONS     = 20;
        inline static constexpr float         GAUSS_SEIDEL_OVERRELAXATION = 1.9f;
        inline static           int32         turbinePower                = 50;
        inline static           bool          pauseSimulation             = true;
        inline static           bool          resetSimulation             = false;
        inline static           bool          activateDebugging           = false;
        inline static           bool          renderObjects               = false;
        inline static           Integrator    integratorChoice            = Integrator::ForwardEuler;
        inline static           Visualization visualizationChoice         = Visualization::Greyscale;

        LiquefiedParams() = delete;
    };

    struct LiquefiedDebug
    {
        inline static LogFloatXY minPressure   = {FLT_MAX, 0, 0};
        inline static LogFloatXY maxPressure   = {FLT_MIN, 0, 0};
        inline static LogFloatXY minDivergence = {FLT_MAX, 0, 0};
        inline static LogFloatXY maxDivergence = {FLT_MIN, 0, 0};
        inline static LogFloatXY minUAdvect    = {FLT_MAX, 0, 0};
        inline static LogFloatXY maxUAdvect    = {FLT_MIN, 0, 0};
        inline static LogFloatXY minVAdvect    = {FLT_MAX, 0, 0};
        inline static LogFloatXY maxVAdvect    = {FLT_MIN, 0, 0};
        inline static float      cflCondition  = 0.0f;

        LiquefiedDebug() = delete;

        static void Reset()
        {
            minPressure   = {FLT_MAX, 0, 0};
            maxPressure   = {FLT_MIN, 0, 0};
            minDivergence = {FLT_MAX, 0, 0};
            maxDivergence = {FLT_MIN, 0, 0};
            minUAdvect    = {FLT_MAX, 0, 0};
            maxUAdvect    = {FLT_MIN, 0, 0};
            minVAdvect    = {FLT_MAX, 0, 0};
            maxVAdvect    = {FLT_MIN, 0, 0};
            cflCondition  = 0.0f;
        }
    };
}
