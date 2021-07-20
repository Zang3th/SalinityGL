#include "../include/WindowManager.hpp"

// ----- Private -----

void WindowManager::CreateWindow()
{
    if(!glfwInit())
        LOG(ERROR) << "GLFW could not be initialized! GLFW_Error: " << glfwGetError(NULL);
    else
        LOG(INFO) << "GLFW initialized!";
        
    glfwWindowHint(GLFW_SAMPLES, 8);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);    

    _window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, _windowName.c_str(), NULL, NULL);

    if(!_window)
        LOG(ERROR) << "Window could not be created! GLFW_Error: " << glfwGetError(NULL);
    else
        LOG(INFO) << "GLFW Window created!";      

    glfwMakeContextCurrent(_window);  
    glfwSwapInterval(0);

    if(!gladLoadGL()) 
        LOG(ERROR) << "Couldn't load OpenGL via glad!";
    else   
        LOG(INFO) << "OpenGL " << GLVersion.major << "." << GLVersion.minor << " loaded via glad!";
  
    GLCall(glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));  
	GLCall(glEnable(GL_MULTISAMPLE));    
    GLCall(glEnable(GL_BLEND)); //Enable blending to render transparent textures
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
   
    _isRunning = true; //Start application
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
        LOG(INFO) << "Quitting!";
    }

    if(glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(_window, true);
        _isRunning = false; 
        LOG(INFO) << "Quitting!";
    }
}

void WindowManager::Prepare()
{
    GLCall(glClearColor(0.0, 0.0, 0.0, 1.0));
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void WindowManager::CalcFrametime()
{
    float currentFrame = glfwGetTime();
	_deltaTime = currentFrame - _lastFrame;
	_lastFrame = currentFrame;
}

// ----- Public -----

WindowManager::WindowManager(const std::string& title)
    : _windowName(title), _window(NULL), _isRunning(false), _deltaTime(0.0f), _lastFrame(0.0f)
{
    CreateWindow();
}

WindowManager::~WindowManager()
{
    glfwTerminate();
}

bool WindowManager::WindowIsRunning()
{
    return _isRunning;
}

void WindowManager::UpdateWindow()
{
    CalcFrametime();
    PollEvents();
    ProcessEvents();
    Prepare();
}

void WindowManager::SwapBuffers()
{
    glfwSwapBuffers(_window);
}