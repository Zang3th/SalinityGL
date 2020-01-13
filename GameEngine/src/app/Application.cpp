#pragma once

#include "../core/DisplayManager.hpp"
#include "../core/GUIManager.hpp"

int main()
{
	DisplayManager displayManager(1800, 1200);
	displayManager.createDisplay();
	displayManager.printVersion();

	GUIManager guiManager;
	guiManager.initializeGUI(displayManager.getWindow());

	while (!displayManager.WindowShouldClose())
	{
		//Backgroundclearcolor
		GLCall(glClearColor(0.070f, 0.098f, 0.427f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

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