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
            float _deltaTime;
            float _lastFrame;

            void CreateWindow();
            void PollEvents();
            void ProcessEvents();
            void Prepare();
            void CalcFrametime();        

        public:
            WindowManager();
            ~WindowManager();
            void SetWindowTitle(const std::string& title);
            bool WindowIsRunning();
            void UpdateWindow();   
            void SwapBuffers();
    };
}