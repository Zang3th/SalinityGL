#pragma once

#include "ErrorManager.hpp"
#include <GLFW/glfw3.h>
#include "CoreDefines.hpp"
#include <string>

namespace Core
{
    class WindowManager
    {
        private:
            std::string _windowName;
            GLFWwindow* _window;
            bool _isRunning;
            double _deltaTime;
            double _lastFrame;
            unsigned int _frameCounter;
            double _dtAccumulated;
            double _fpsAvg;

            void CreateWindow();
            void PollEvents();
            void ProcessEvents();
            void Prepare();
            void CalcFrametime();    

        public:
            WindowManager();
            ~WindowManager();
            void SetWindowTitle(const std::string& title);
            bool WindowIsRunning() const;
            void PrepareFrame();   
            void SwapBuffers();
            double GetDeltaTime() const;
            double GetFps() const;
            unsigned int GetFrameCounter() const;
            GLFWwindow* GetWindow();
    };
}