#pragma once

#include "ErrorManager.hpp"
#include "glfw3.h"
#include "Types.hpp"
#include "GlobalParams.hpp"
#include "GLRenderSettings.hpp"

#include <string>

namespace Engine
{
    class Window
    {
        private:
            inline static GLFWwindow* _window        = nullptr;
            inline static std::string _windowName    = std::string();
            inline static double      _deltaTime_sec = double();
            inline static double      _lastFrame     = double();
            inline static double      _dtAccumulated = double();
            inline static double      _fpsAvg        = double();
            inline static uint32      _frameCounter  = uint32();
            inline static bool        _isRunning     = bool();

        public:
            Window() = delete;
            static uint32 Init(const std::string& title);
            static void Close();
            static void CalcFrametime();
            static void PollEvents();
            static void ProcessEvents();
            static void SwapBuffers();

            [[nodiscard]] static bool        IsRunning();
            [[nodiscard]] static double      GetDeltaTime_sec();
            [[nodiscard]] static double      GetDeltaTime_msec();
            [[nodiscard]] static double      GetFps();
            [[nodiscard]] static uint32      GetFrameCounter();
            [[nodiscard]] static GLFWwindow* GetWindow();
            [[nodiscard]] static float       GetAspectRatio();
    };
}