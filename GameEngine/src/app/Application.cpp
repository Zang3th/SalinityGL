#include "DisplayManager.hpp"
#include "GUIManager.hpp"
#include "Renderer.hpp"
#include "ModelManager.hpp"
#include "AudioManager.hpp"
#include "MousePicker.hpp"

bool renderRay = false;
bool fixedRay = false;
bool calcCollision = false;

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
	MousePicker mousePicker(modelManager.getTerrain());

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

		//Update RenderRay Position		
		if(!fixedRay)
		{
			glm::vec3 camPos = glm::vec3(_camera->Position.x, _camera->Position.y - 1, _camera->Position.z);
			modelManager.createRay(camPos, _camera->Front, _camera->Yaw, renderRay, 1000.0f, 0.1f);
		}

		if(calcCollision)
		{
			mousePicker.calculateTerrainEntry(_camera->Position, _camera->Front);
			modelManager.colorPickedVertices(mousePicker._mouseRayTerrainEntry);
		}
		
		//Render Stuff		
		renderer.render(modelManager.Models);

		//GUI Stuff
		{
			//Neues Fenster mit FPS Counter rendern
			guiManager.newWindow("General stuff");
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::Text("---------------------------------------------");
			ImGui::Text("Camera-Coords: X: %f, Y: %f, Z: %f", _camera->Position.x, _camera->Position.y, _camera->Position.z);
			ImGui::Text("Camera-Yaw: %f, Camera-Pitch: %f", _camera->Yaw, _camera->Pitch);
			ImGui::Text("Camera-Front: X: %f, Y: %f, Z: %f", _camera->Front.x, _camera->Front.y, _camera->Front.z);
			ImGui::Text("---------------------------------------------");
			//ImGui::Text("Player-Coords: X: %f, Y: %f, Z: %f", displayManager._player->_playerPosition.x, displayManager._player->_playerPosition.y, displayManager._player->_playerPosition.z);
			//ImGui::Text("Player-Rotation: %f", displayManager._player->_yaw);
			ImGui::Text("---------------------------------------------");
			ImGui::Checkbox("Mouse-Ray rendern", &renderRay); ImGui::SameLine(); ImGui::Checkbox("Mouse-Ray fixieren", &fixedRay);
			ImGui::Text("Mouse-Coords: X: %f, Y: %f", rawMouse_X, rawMouse_Y);
			ImGui::Text("Mouse-Ray-Direction: X: %f, Y: %f, Z: %f", mousePicker._mouseRay.x, mousePicker._mouseRay.y, mousePicker._mouseRay.z);
			ImGui::Checkbox("Terrain Collision", &calcCollision);
			ImGui::Text("Terrain-Entry-Point: X: %f, Y: %f, Z: %f", mousePicker._mouseRayTerrainEntry.x, mousePicker._mouseRayTerrainEntry.y, mousePicker._mouseRayTerrainEntry.z);
			ImGui::Text("---------------------------------------------");
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