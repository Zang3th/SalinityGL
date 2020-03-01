#pragma once

#include "DisplayManager.hpp"
#include "GUIManager.hpp"
#include "Renderer.hpp"
#include "ModelManager.hpp"
#include "AudioManager.hpp"
#include "MousePicker.hpp"

int main()
{
	Player* _playerObject = nullptr; //Player object	

	//Display-Management
	DisplayManager displayManager(_playerObject);
	displayManager.createDisplay();
	displayManager.printVersion();

	//GUI-Management
	GUIManager guiManager;
	guiManager.initializeGUI(displayManager.getWindow());

	//Sound-Management
	AudioManager audioManager;
	audioManager.playSound2D("res/audio/music/TrueBlueSky.mp3", true);

	//Renderer
	Renderer renderer;	

	//Modelmanager
	ModelManager modelManager(&displayManager, &audioManager);	

	//Mousepicker
	MousePicker mousePicker;

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

		//Update MouseRay
		mousePicker.update();

		//Render Stuff		
		renderer.render(modelManager.Models);

		//GUI Stuff
		{
			//Neues Fenster mit FPS Counter rendern
			guiManager.newWindow("General stuff");
			ImGui::Text("\nApplication average %.3f ms/frame (%.1f FPS)\n", 1000.f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::Text("Camera-Coords: X: %f, Y: %f, Z: %f", _camera->Position.x, _camera->Position.y, _camera->Position.z);
			ImGui::Text("Camera-Yaw: %f, Camera-Pitch: %f", _camera->Yaw, _camera->Pitch);			
			ImGui::Text("Player-Coords: X: %f, Y: %f, Z: %f", displayManager._player->_playerPosition.x, displayManager._player->_playerPosition.y, displayManager._player->_playerPosition.z);
			ImGui::Text("Player-Rotation: %f", displayManager._player->_yaw);
			ImGui::Text("Mouse-Coords (VIEW): X: %f, Y: %f", rawMouse_X, rawMouse_Y);
			ImGui::Text("Mouse-RAY-Direction (WORLD): X: %f, Y: %f, Z: %f", mousePicker._mouseRay.x, mousePicker._mouseRay.y, mousePicker._mouseRay.z);
			guiManager.exitWindow();
		}

		//Update stuff
		{
			audioManager.updateListenerPosition(&_camera->Position, &_camera->Front, &_camera->Up);
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