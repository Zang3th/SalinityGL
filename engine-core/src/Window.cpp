#include "Window.hpp"

namespace Core
{
    // ----- Private -----

    void Window::CreateWindow()
    {
        if(!glfwInit())
            LOG(ERROR) << "Failed:   GLFW-Initialization | " << glfwGetError(nullptr);
        else
            LOG(INFO) << "Loaded:   GLFW";
            
        glfwWindowHint(GLFW_SAMPLES, 8);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        glfwWindowHint(GLFW_MAXIMIZED, GLFW_FALSE);
        glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

        _window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, _windowName.c_str(), nullptr, nullptr);

        if(!_window)
            LOG(ERROR) << "Failed:   GLFW-Window-Creation | " << glfwGetError(nullptr);
        else
            LOG(INFO) << "Created:  GLFW-Window (" << WINDOW_WIDTH << "x" << WINDOW_HEIGHT << ")";

        glfwMakeContextCurrent(_window);  
        glfwSwapInterval(1);

        if(!gladLoadGL()) 
            LOG(ERROR) << "Failed:   OpenGL-Loading with glad";
        else
            LOG(INFO) << "Loaded:   OpenGL with glad | " << glGetString(GL_VENDOR) << " | " << glGetString(GL_RENDERER) << " | " << glGetString(GL_VERSION);
    
        GLCall(glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));  
        GLCall(glEnable(GL_MULTISAMPLE));
        GLCall(glEnable(GL_DEPTH_TEST));
        GLCall(glDepthFunc(GL_LEQUAL));
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        //Enable extensive debugging information (available since OpenGL 4.3)
        GLCall(glEnable(GL_DEBUG_OUTPUT));
        GLCall(glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS));
        GLCall(glDebugMessageCallback(ErrorManager::OpenGLMessageCallback, nullptr));
        GLCall(glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE));
    
        _isRunning = true; //Start application
    }

    // ----- Public -----

    Window::Window()
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

    Window::~Window()
    {
        glfwTerminate();
    }

    void Window::SetTitle(const std::string& title)
    {
        _windowName = title;
        glfwSetWindowTitle(_window, _windowName.c_str());
        LOG(INFO) << "Changed:  Window-Title | " << title;
    }

    bool Window::IsRunning() const
    {
        return _isRunning;
    }

    void Window::CalcFrametime()
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

    void Window::PollEvents()
    {
        glfwPollEvents();
    }

    void Window::ProcessEvents()
    {
        if(glfwWindowShouldClose(_window))
        {
            _isRunning = false;
            LOG(INFO) << "Closed:   " << _windowName;
        }

        if(glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(_window, true);
            _isRunning = false;
            LOG(INFO) << "Closed:   " << _windowName;
        }
    }

    void Window::SwapBuffers()
    {
        glfwSwapBuffers(_window);
    }


    double Window::GetDeltaTime() const
    {
        return _deltaTime;
    }

    double Window::GetFps() const
    {
        return _fpsAvg;
    }

    uint32 Window::GetFrameCounter() const
    {
        return _frameCounter;
    }

    GLFWwindow* Window::GetWindow() const
    {
        return _window;
    }
}