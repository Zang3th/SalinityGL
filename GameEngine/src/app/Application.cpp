#pragma once

#include "DisplayManager.hpp"
#include "GUIManager.hpp"
#include "Model.hpp"
#include "Renderer.hpp"
#include "Shader.hpp"

int main()
{
	//Display-Management
	DisplayManager displayManager(1800, 1200);
	displayManager.createDisplay();
	displayManager.printVersion();

	//GUI-Management
	GUIManager guiManager;
	guiManager.initializeGUI(displayManager.getWindow());

	//Renderer
	Renderer renderer;

	//Data
	float vertices[] = {
	 -0.5f,  0.5f, 0.0f,  // top right
	 -0.5f, -0.5f, 0.0f,  // bottom right
	  0.5f, -0.5f, 0.0f,  // bottom left
	  0.5f,  0.5f, 0.0f   // top left 
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};
	
	//Modelcreation
	Model cube(vertices, sizeof(vertices), indices, sizeof(indices));
	
	//Shaderstuff
	Shader cube_shader("res/shader/vertexShader.glsl", "res/shader/fragmentShader.glsl");

	while (!displayManager.WindowShouldClose())
	{
		//Clear Buffer and prepare for rendering
		renderer.prepare();
		
		//Render Stuff
		cube_shader.bind();
		renderer.render(&cube);
		cube_shader.unbind();

		//GUI Stuff
		{
			guiManager.startFrame();

			//Neues Fenster mit FPS Counter rendern
			guiManager.newWindow("General settings");
			guiManager.printFPS();
			guiManager.exitWindow();
		}

		//Update stuff
		{
			guiManager.renderGUI();
			displayManager.updateDisplay();
		}
	}
	guiManager.cleanUPGUI();
	displayManager.closeDisplay();

	return 0;
}