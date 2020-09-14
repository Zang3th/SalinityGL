#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Game.hpp"
#include <spdlog/spdlog.h>
#include "OpenGLErrorManager.hpp"

float deltaTime = 0.0f;	//Time between current frame and last frame
float lastFrame = 0.0f; //Time of last frame

const unsigned int WIDTH = 1920; //Global WIDTH-Setting
const unsigned int HEIGHT = 1080; //Global HEIGHT-Setting

Game breakout(WIDTH, HEIGHT); //Game creation

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// when a user presses the escape key, we set the WindowShouldClose property to true, closing the application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			breakout._keys[key] = true;
		else if (action == GLFW_RELEASE)
			breakout._keys[key] = false;
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

class GameDisplayManager
{
private:
	int _width, _height;
	GLFWwindow* _window = nullptr;

public:
	GameDisplayManager(int width, int height)
		: _width(width), _height(height)
	{
		this->createDisplay();
	}

	~GameDisplayManager()
	{

	}

	void createDisplay()
	{
		if (!glfwInit())
			spdlog::error("GLFW INIT ERROR\n");
		
		glfwWindowHint(GLFW_SAMPLES, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		_window = glfwCreateWindow(_width, _height, "Breakout", NULL, NULL);

		if (!_window)
			spdlog::error("GLFW WINDOW CREATION ERROR\n");

		glfwMakeContextCurrent(_window);
		glfwSwapInterval(0);

		if (glewInit() != GLEW_OK)
			spdlog::error("GLEW INIT ERROR\n");

		GLCall(glViewport(0, 0, _width, _height)); //Renderscreensize
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		GLCall(glEnable(GL_MULTISAMPLE)); //Multisampling
		
		glfwSetKeyCallback(_window, key_callback);
		glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);
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
	}

	void pollEvents()
	{
		glfwPollEvents();
	}

	void closeDisplay()
	{
		glfwTerminate();
	}

	GLFWwindow* getWindow()
	{
		return _window;
	}

	void measureFrameTime()
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
	}

	void clear()
	{
		GLCall(glClearColor(0.450f, 0.450f, 0.450f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
	}
};