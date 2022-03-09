#pragma once

#include "ErrorManager.hpp"
#include "glfw3.h"
#include "CoreDefines.hpp"

namespace Engine
{
    class Window
    {
        private:
            GLFWwindow*     _window;
            std::string     _windowName;
            double          _deltaTime;
            double          _lastFrame;
            double          _dtAccumulated;
            double          _fpsAvg;
            uint32          _frameCounter;
            bool            _isRunning;

            void CreateWindow();
            void SetTitle(const std::string& title);

        public:
            explicit Window(const std::string& title);
            ~Window();
            [[nodiscard]] bool IsRunning() const;
            void CalcFrametime();
            static void PollEvents();
            void ProcessEvents();
            void SwapBuffers();
            [[nodiscard]] double GetDeltaTime() const;
            [[nodiscard]] double GetFps() const;
            [[nodiscard]] uint32 GetFrameCounter() const;
            [[nodiscard]] GLFWwindow* GetWindow() const;
    };
}