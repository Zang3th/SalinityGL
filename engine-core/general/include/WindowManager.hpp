#pragma once

#include "ErrorManager.hpp"
#include <GLFW/glfw3.h>
#include "GlobalDefine.hpp"
#include <string>

class WindowManager
{
    private:
        void CreateWindow();
        void PollEvents();
        void ProcessEvents();
        void Prepare();
        void CalcFrametime();

        const char* _windowName;
        GLFWwindow* _window;
        bool _isRunning;
        float _deltaTime;
        float _lastFrame;

    public:
        WindowManager(const char* title);
        ~WindowManager();
        bool WindowIsRunning();
        void UpdateWindow();   
        void SwapBuffer();
};