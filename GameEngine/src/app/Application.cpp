#pragma once

#include "DisplayManager.hpp"
#include "GUIManager.hpp"
#include "Model.hpp"
#include "Renderer.hpp"
#include "Shader.hpp"
#include "GroundData.hpp"
#include "Filemanager.hpp"
#include "AssimpLoader.hpp"
#include "Texture.hpp"

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
	Shader standard_shader("res/shader/standard_vs.glsl", "res/shader/standard_fs.glsl");
	Shader ground_shader("res/shader/ground_vs.glsl", "res/shader/ground_fs.glsl");

	//Data
	GroundData ground_data(128);
	AssimpLoader cube_data("res/obj/Shuttle.obj");
	AssimpLoader axe_data("res/obj/Axe.obj");
	AssimpLoader dolphin_data("res/obj/Dolphin.obj");

	//Textures
	Texture shuttle_tex("res/textures/shuttle.jpg");
	Texture axe_tex("res/textures/Axe.jpg");
	Texture bark_tex("res/textures/Bark.jpg");
	Texture grass_tex("res/textures/Grass.jpg");
	Texture dolphin_tex("res/textures/Dolphin.jpg");
	shuttle_tex.bind(0);
	axe_tex.bind(1);
	bark_tex.bind(2);
	grass_tex.bind(3);
	dolphin_tex.bind(4);

	//Modelmanager
	std::vector<Model*> Models;

	//Modelcreation
	Model shuttle(&cube_data, &standard_shader, 0);
	shuttle.translate(glm::vec3(5.0f, 0.0f, 0.0f));
	Models.push_back(&shuttle);

	Model axe(&axe_data, &standard_shader, 1);
	Models.push_back(&axe);

	Model dolphin(&dolphin_data, &standard_shader, 4);
	Models.push_back(&dolphin);

	Model ground(&ground_data, &ground_shader, 3);
	ground.translate(glm::vec3(0.0f, -5.0f, 0.0f));
	Models.push_back(&ground);

	//DEBUG via files
	Filemanager filemanager;
	{
		/*filemanager.writeReadableToFile(wall_data._vertices, "res/data/wallvertices.data");
		filemanager.writeReadableToFile(wall_data._texCoords, "res/data/walltexcoords.data");
		filemanager.writeReadableToFile(wall_data._indices, "res/data/wallindices.data");*/
		/*filemanager.writeReadableToFile(test_data._vertices, "res/data/testvertices.data");
		filemanager.writeReadableToFile(test_data._texCoords, "res/data/testtexcoords.data");
		
		filemanager.writeReadableToFile(test_data._indices, "res/data/testindices.data");*/
	}

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
	{
		guiManager.cleanUPGUI();
		displayManager.closeDisplay();
	}
	
	return 0;
}