#pragma once

#include "DisplayManager.hpp"
#include "GUIManager.hpp"
#include "Renderer.hpp"
#include "ModelManager.hpp"

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

	//Modelmanager
	ModelManager modelManager;	

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
		renderer.render(modelManager.Models);

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
	{
		guiManager.cleanUPGUI();
		displayManager.closeDisplay();
	}
	
	return 0;
}