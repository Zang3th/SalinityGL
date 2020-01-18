#pragma once

#include "DisplayManager.hpp"
#include "GUIManager.hpp"
#include "Model.hpp"
#include "Renderer.hpp"
#include "Shader.hpp"
#include "RawData.hpp"

int main()
{
	//Display-Management
	DisplayManager displayManager;
	displayManager.createDisplay();
	displayManager.printVersion();

	//GUI-Management
	GUIManager guiManager;
	guiManager.initializeGUI(displayManager.getWindow());

	//Renderer
	Renderer renderer;

	//Shaderstuff
	Shader cube_shader("res/shader/vertexShader.glsl", "res/shader/fragmentShader.glsl");

	//Data
	RawData cube_data;
	cube_data.initializeCubeData();	

	//Modelcreation
	std::vector<Model*> Models;

	Model cube_1(&cube_data, &cube_shader);
	cube_1.translate(glm::vec3(1.0f, 0.0f, 0.0f));
	Models.push_back(&cube_1);

	Model cube_2(&cube_data, &cube_shader);
	cube_2.translate(glm::vec3(-1.0f, 0.0f, 0.0f));
	Models.push_back(&cube_2);	

	while (!displayManager.WindowShouldClose())
	{
		//Measure Frametime
		displayManager.measureFrameTime();

		//Clear Buffer and prepare for rendering
		renderer.prepare();

		//Start GUI-Frame
		guiManager.startFrame();

		//Check Keyboard and Mouseinputs
		displayManager.checkForInput();
		
		//Render Stuff		
		renderer.render(Models);

		//GUI Stuff
		{
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
	//CleanUP Stuff
	guiManager.cleanUPGUI();
	displayManager.closeDisplay();

	return 0;
}