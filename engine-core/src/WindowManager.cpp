#include "WindowManager.hpp"

namespace Core
{
    // ----- Private -----

    void WindowManager::CreateWindow()
    {
        if(!glfwInit())
            LOG(ERROR) << "Failed:   GLFW-Initialization | " << glfwGetError(nullptr);
        else
            LOG(INFO) << "Loaded:   GLFW";
            
        glfwWindowHint(GLFW_SAMPLES, 8);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);    

        _window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, _windowName.c_str(), nullptr, nullptr);

        if(!_window)
            LOG(ERROR) << "Failed:   GLFW-Window-Creation | " << glfwGetError(nullptr);
        else
            LOG(INFO) << "Created:  GLFW-Window";

        glfwMakeContextCurrent(_window);  
        glfwSwapInterval(1);

        if(!gladLoadGL()) 
            LOG(ERROR) << "Failed:   OpenGL-Loading with glad";
        else
            LOG(INFO) << "Loaded:   OpenGL " << GLVersion.major << "." << GLVersion.minor << " with glad";
    
        GLCall(glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));  
        GLCall(glEnable(GL_MULTISAMPLE));
        GLCall(glEnable(GL_DEPTH_TEST));
        GLCall(glDepthFunc(GL_LEQUAL));
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    
        _isRunning = true; //Start application
    }

    // ----- Public -----

    WindowManager::WindowManager()
        :   _window(nullptr),
            _windowName("GameEngine-Default"),
            _deltaTime(0.0f),
            _lastFrame(0.0f),
            _dtAccumulated(0.0f),
            _fpsAvg(0.0f),
            _frameCounter(0),
            _isRunning(false)
    {
        CreateWindow();
    }

    WindowManager::~WindowManager()
    {
        glfwTerminate();
    }

    void WindowManager::SetWindowTitle(const std::string& title)
    {
        _windowName = title;
        glfwSetWindowTitle(_window, _windowName.c_str());
    }

    bool WindowManager::WindowIsRunning() const
    {
        return _isRunning;
    }

    void WindowManager::Prepare()
    {
        GLCall(glClearColor(0.0, 0.0, 0.0, 1.0));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    }

    void WindowManager::CalcFrametime()
    {
        //Calculate frametime
        double currentFrame = glfwGetTime();
        _deltaTime = currentFrame - _lastFrame;
        _lastFrame = currentFrame;

        //Accumulate to average the fps
        _frameCounter++;
        _dtAccumulated += _deltaTime;

        if(_frameCounter > 160)
        {
            _fpsAvg = 1 / (_dtAccumulated / _frameCounter);

            //Reset
            _frameCounter = 0;
            _dtAccumulated = 0.0f;
        }
    }

    void WindowManager::PollEvents()
    {
        glfwPollEvents();
    }

    void WindowManager::ProcessEvents()
    {
        if(glfwWindowShouldClose(_window))
        {
            _isRunning = false;
            LOG(INFO) << "Quit application";
        }

        if(glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(_window, true);
            _isRunning = false;
            LOG(INFO) << "Quit application";
        }
    }

    void WindowManager::SwapBuffers()
    {
        glfwSwapBuffers(_window);
    }


    double WindowManager::GetDeltaTime() const
    {
        return _deltaTime;
    }

    double WindowManager::GetFps() const
    {
        return _fpsAvg;
    }

    uint32 WindowManager::GetFrameCounter() const
    {
        return _frameCounter;
    }

    GLFWwindow* WindowManager::GetWindow() const
    {
        return _window;
    }
}