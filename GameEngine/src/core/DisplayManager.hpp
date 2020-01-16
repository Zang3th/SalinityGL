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
float lastX = WIDTH / 2.0f; //X-Coord of last frame
float lastY = HEIGHT / 2.0f; //Y-Coord of last frame
bool window_focused = false; //Is the window in focus?
Camera* _camera = nullptr; //Global camera object. Needs to be global for callbacks and Model.hpp

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		_camera->ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		_camera->ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		_camera->ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		_camera->ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		_camera->ProcessKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		_camera->ProcessKeyboard(DOWN, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (window_focused)
	{
		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

		lastX = xpos;
		lastY = ypos;
		_camera->ProcessMouseMovement(xoffset, yoffset);
	}
	else
	{
		lastX = xpos;
		lastY = ypos;
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	_camera->ProcessMouseScroll(yoffset);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		window_focused = true;
	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		window_focused = false;
	}
}

class DisplayManager
{
private:
	int _width, _height;	
	GLFWwindow* _window = nullptr;	

public:
	DisplayManager()
		: _width(WIDTH), _height(HEIGHT)
	{
		_camera = new Camera(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	~DisplayManager()
	{
		delete _camera;
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

		glfwSetCursorPosCallback(_window, mouse_callback);
		glfwSetScrollCallback(_window, scroll_callback);
		glfwSetMouseButtonCallback(_window, mouse_button_callback);
		GLCall(glEnable(GL_DEPTH_TEST)); //Depthtesting
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
		//Measure deltaTime
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(_window);
		glfwSwapBuffers(_window);
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
};