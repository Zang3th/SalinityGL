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
	DisplayManager displayManager(1800, 1200);
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
	Model cube(&cube_data, &cube_shader);

	while (!displayManager.WindowShouldClose())
	{
		//Clear Buffer and prepare for rendering
		renderer.prepare();

		//Render Stuff
		renderer.render(&cube);

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