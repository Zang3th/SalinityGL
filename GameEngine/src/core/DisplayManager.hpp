#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include "OpenGLErrorManager.hpp"

class DisplayManager
{
private:
	int _width, _height;
	GLFWwindow* _window;

public:
	DisplayManager(int width, int height)
		: _width(width), _height(height)
	{
		
	}

	void createDisplay()
	{
		if (!glfwInit())
			spdlog::error("GLFW INIT ERROR\n");

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		_window = glfwCreateWindow(_width, _height, "GameEngine", NULL, NULL);
	
		if (!_window)
			spdlog::error("GLFW WINDOW CREATION ERROR\n");

		glfwMakeContextCurrent(_window);
		glfwSwapInterval(0);

		if (glewInit() != GLEW_OK)
			spdlog::error("GLFW WINDOW CREATION ERROR\n");

		GLCall(glViewport(0, 0, _width, _height)); //Renderscreensize

		//glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); //Resize with window	
		//glfwSetCursorPosCallback(window, mouse_callback);
		//glfwSetScrollCallback(window, scroll_callback);
		//glfwSetMouseButtonCallback(window, mouse_button_callback);
		//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); //Set Standardmousecontrol to normal
		//GLCall(glEnable(GL_DEPTH_TEST)); //Depthtesting
	}

	void printVersion()
	{
		GLCall(const unsigned char* gl_ver = glGetString(GL_VERSION));
		spdlog::info(gl_ver);
	}

	int WindowShouldClose()
	{
		return glfwWindowShouldClose(_window);
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