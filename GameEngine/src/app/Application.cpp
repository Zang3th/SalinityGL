#include "DisplayManager.hpp"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include "AudioManager.hpp"
#include "MousePicker.hpp"
#include "EntityManager.hpp"

bool renderRay = false;
bool fixedRay = false;
bool terrainEditor = false;
bool deleteLastColoredVert = false;
bool raise = false;
bool sink = false;

int main()
{
	Player* _playerObject = nullptr; //Player object	

	//Display-Management
	DisplayManager displayManager(_playerObject);
	displayManager.createDisplay();
	displayManager.printVersion();
	
	//Setup ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark(); //Setup ImGui style	
	ImGui_ImplGlfw_InitForOpenGL(displayManager.getWindow(), true); //Setup Platform/Renderer bindings
	ImGui_ImplOpenGL3_Init("#version 330");

	//Audio-Management
	AudioManager audioManager;
	//audioManager.playSound2D("res/audio/music/TrueBlueSky.mp3", true);

	//Create Models ----------------------------------------------------------------------------------------------------------------------------------------------------	
	EntityManager entityManager;

	//Cubemap
	entityManager.addCubemapEntity();
	
	//Ground
	auto ground = entityManager.addTerrainEntity(512, 1, "res/maps/Heightmap_512.bmp", "res/textures/Grass.jpg", "res/textures/Dirt.jpg", "res/maps/Blendmap_512.jpg");

	//Water
	auto water = entityManager.addPlaneEntity(160, 1, "res/textures/Water.jpg");
	water->translate(glm::vec3(190, -4.5, 480));
	water->rotate(90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	
	//House
	auto house = entityManager.addOBJEntity("res/obj/houses/Farmhouse.obj", "res/textures/models/Farmhouse.jpg");
	house->translate(glm::vec3(440, -1.5, 140));
	house->scale(glm::vec3(2.25f, 2.25f, 2.25f));
	house->rotate(170.0f, glm::vec3(0, 1, 0));
	house->rotate(-5.0f, glm::vec3(1, 0, 0));
	house->rotate(-3.0f, glm::vec3(0, 0, 1));

	//Wood
	auto wood = entityManager.addOBJEntity("res/obj/vegetation/Wood.obj", "res/textures/models/Wood.jpg");
	wood->translate(glm::vec3(412, -3.0, 116));
	wood->scale(glm::vec3(2.5f, 2.5f, 2.5f));

	//Axe
	auto axe = entityManager.addOBJEntity("res/obj/tools/Axe.obj", "res/textures/models/Axe.jpg");
	axe->translate(glm::vec3(408, 3.5, 114));
	axe->scale(glm::vec3(0.4f, 0.4f, 0.4f));
	axe->rotate(120.0f, glm::vec3(0, -1, 0));
	axe->rotate(120.0f, glm::vec3(0, 0, -1));

	//Grass
	//auto grass = entityManager.addOBJEntity("res/obj/vegetation/LowGrass.obj", "res/textures/models/GrassOBJ.jpg");

	//Tree
	//auto tree = entityManager.addTreeEntity();

	//Lights
	//auto light = entityManager.addLightEntity("res/obj/lightsources/Parklight.obj", "res/textures/models/Metal_2_dark.jpg", "res/obj/geometry/cylinder.obj", "res/textures/models/White.jpg");
	//------------------------------------------------------------------------------------------------------------------------------------------------------------------	
	//Mousepicker
	MousePicker mousePicker(ground);

	//Create ray visualization
	entityManager.createRay();
	
	while (!displayManager.WindowShouldClose())
	{
		//Measure Frametime
		displayManager.measureFrameTime();
		
		//Start GUI-Frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		//Check Keyboard and Mouseinputs
		displayManager.checkForInput();
		
		//Update MouseRay
		mousePicker.update();
		
		//Update RenderRay Position
		if(!fixedRay)
		{
			glm::vec3 camPos = glm::vec3(_camera->Position.x, _camera->Position.y - 1, _camera->Position.z);
			entityManager.visualizeRay(camPos, _camera->Front, _camera->Yaw, renderRay, 1000.0f, 0.1f);
		}
		else if(!renderRay)
		{
			fixedRay = false;
			glm::vec3 camPos = glm::vec3(_camera->Position.x, _camera->Position.y - 1, _camera->Position.z);
			entityManager.visualizeRay(camPos, _camera->Front, _camera->Yaw, renderRay, 1000.0f, 0.1f);
		}			
					
		//Activate Terraineditor
		if(terrainEditor)
		{
			mousePicker.calculateTerrainEntry(_camera->Position, _camera->Front);
			entityManager.colorPickedVertice(mousePicker._mouseRayTerrainEntry);
			deleteLastColoredVert = true;
			
			if(mouseIsHold)
			{
				if(raise == true && sink == false && ignoreNextClick == false)
				{
					entityManager.raiseTerrain();
				}
				else if(sink == true && raise == false && ignoreNextClick == false)
				{
					entityManager.sinkTerrain();
				}				
			}
		}
		else
		{
			raise = false;
			sink = false;
		}

		//Otherwise the last picked Vertice would remain white
		if(terrainEditor == false && deleteLastColoredVert == true)
		{
			entityManager.deleteLastColoredVertice();
			deleteLastColoredVert = false;
		}		
		
		//Render models
		entityManager.render();
		
		//GUI Stuff
		{
			ImGui::Begin("General stuff");
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
			ImGui::Text("---------------------------------------------");
			ImGui::Checkbox("Terrain-Editor", &terrainEditor); ImGui::SameLine(); ImGui::Checkbox("Raise", &raise); ImGui::SameLine(); ImGui::Checkbox("Sink", &sink);
			ImGui::Text("Terrain-Entry-Point: X: %f, Y: %f, Z: %f", mousePicker._mouseRayTerrainEntry.x, mousePicker._mouseRayTerrainEntry.y, mousePicker._mouseRayTerrainEntry.z);
			ImGui::Text("---------------------------------------------");
			ImGui::End();
		}

		//Update stuff
		{
			//audioManager.updateListenerPosition(&_camera->Position, &_camera->Front, &_camera->Up);
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			displayManager.updateDisplay();
		}		
	}
	
	//CleanUP Stuff
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		displayManager.closeDisplay();
	}
	
	return 0;
}