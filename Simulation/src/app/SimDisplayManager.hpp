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

Camera camera(glm::vec3(-40.0f, 50.0f, -14.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Global camera object -> global because of callbacks

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (window_focused)
	{
		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

		lastX = xpos;
		lastY = ypos;
		camera.ProcessMouseMovement(xoffset, yoffset);
	}
	else
	{
		lastX = xpos;
		lastY = ypos;
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
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

class SimDisplayManager
{
private:
	GLFWwindow* _window = nullptr;
	
public:	
	void createDisplay()
	{
		if (!glfwInit())
			spdlog::error("GLFW INIT ERROR\n");

		glfwWindowHint(GLFW_SAMPLES, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
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
		glfwSetCursorPosCallback(_window, mouse_callback);
		glfwSetScrollCallback(_window, scroll_callback);
		glfwSetMouseButtonCallback(_window, mouse_button_callback);
		GLCall(glEnable(GL_DEPTH_TEST)); //Depthtesting
		GLCall(glEnable(GL_MULTISAMPLE)); //Multisampling
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
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}

	void updateDisplay()
	{
		glfwSwapBuffers(_window);
		glfwPollEvents();
	}	

	void processInput()
	{
		if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(_window, true);

		if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS)
			camera.ProcessKeyboard(FORWARD, deltaTime);
		if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS)
			camera.ProcessKeyboard(BACKWARD, deltaTime);
		if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS)
			camera.ProcessKeyboard(LEFT, deltaTime);
		if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS)
			camera.ProcessKeyboard(RIGHT, deltaTime);
		if (glfwGetKey(_window, GLFW_KEY_SPACE) == GLFW_PRESS)
			camera.ProcessKeyboard(UP, deltaTime);
		if (glfwGetKey(_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
			camera.ProcessKeyboard(DOWN, deltaTime);
	}

	void closeDisplay()
	{
		glfwTerminate();
	}
};