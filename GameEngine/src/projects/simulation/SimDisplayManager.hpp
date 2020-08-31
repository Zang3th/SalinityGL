#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include "OpenGLErrorManager.hpp"
#include "Camera.hpp"

const unsigned int WIDTH = 1800; //Global WIDTH-Setting
const unsigned int HEIGHT = 1200; //Global HEIGHT-Setting

float deltaTime = 0.0f;	//Time between current frame and last frame
float lastFrame = 0.0f; //Time of last frame
float lastX = WIDTH / 2.0f; //X-Coord of the mouse
float lastY = HEIGHT / 2.0f; //Y-Coord of the mouse
bool window_focused = false; //Is the window in focus?

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

class SimDisplayManager
{
private:
	GLFWwindow* _window = nullptr;
	
public:	
	void createDisplay()
	{
		if (!glfwInit())
			spdlog::error("GLFW INIT ERROR\n");

		//glfwWindowHint(GLFW_SAMPLES, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		_window = glfwCreateWindow(WIDTH, HEIGHT, "Simulation", NULL, NULL);

		if (!_window)
			spdlog::error("GLFW WINDOW CREATION ERROR\n");

		glfwMakeContextCurrent(_window);
		glfwSwapInterval(0);

		if (glewInit() != GLEW_OK)
			spdlog::error("GLEW INIT ERROR\n");

		GLCall(glViewport(0, 0, WIDTH, HEIGHT)); //Renderscreensize
		
		glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback); //Resize framebuffer
		//glfwSetCursorPosCallback(_window, mouse_callback);
		//glfwSetScrollCallback(_window, scroll_callback);
		//glfwSetMouseButtonCallback(_window, mouse_button_callback);
		//GLCall(glEnable(GL_DEPTH_TEST)); //Depthtesting
		//GLCall(glEnable(GL_MULTISAMPLE)); //Multisampling
		//GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
	}
	
	SimDisplayManager()
	{
		
	}

	~SimDisplayManager()
	{
		
	}

	void printVersion()
	{
		GLCall(const unsigned char* gl_ver = glGetString(GL_VERSION));
		spdlog::info(gl_ver);
	}

	GLFWwindow* getWindow()
	{
		return _window;
	}

	int windowShouldClose()
	{
		return glfwWindowShouldClose(_window);
	}

	void measureFrameTime()
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
	}

	void clear()
	{
		GLCall(glClearColor(0.368f, 0.721f, 0.811f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}

	void updateDisplay()
	{
		glfwSwapBuffers(_window);
		glfwPollEvents();
	}

	void closeDisplay()
	{
		glfwTerminate();
	}
};