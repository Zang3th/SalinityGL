#pragma once

#include "ErrorManager.hpp"
#include "glfw3.h"
#include "CoreDefines.hpp"

namespace Core
{
    class WindowManager
    {
        private:
            GLFWwindow*     _window;
            std::string     _windowName;
            double          _deltaTime;
            double          _lastFrame;
            double          _dtAccumulated;
            double          _fpsAvg;
            unsigned int    _frameCounter;
            bool            _isRunning;

            void CreateWindow();
            static void Prepare();
            void CalcFrametime();    

        public:
            WindowManager();
            ~WindowManager();
            void SetWindowTitle(const std::string& title);
            [[nodiscard]] bool WindowIsRunning() const;
            void PrepareFrame();
            static void PollEvents();
            void ProcessEvents();
            void SwapBuffers();
            [[nodiscard]] double GetDeltaTime() const;
            [[nodiscard]] double GetFps() const;
            [[nodiscard]] unsigned int GetFrameCounter() const;
            [[nodiscard]] GLFWwindow* GetWindow() const;
    };
}