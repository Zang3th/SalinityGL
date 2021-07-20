#pragma once

#include "ErrorManager.hpp"
#include <GLFW/glfw3.h>
#include "GlobalDefine.hpp"
#include <string>

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
        WindowManager(const std::string& title);
        ~WindowManager();
        bool WindowIsRunning();
        void UpdateWindow();   
        void SwapBuffers();
};