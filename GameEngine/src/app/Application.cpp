#include "DisplayManager.hpp"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
//#include "Renderer.hpp"
//#include "ModelManager.hpp"
#include "AudioManager.hpp"
//#include "MousePicker.hpp"
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
	audioManager.playSound2D("res/audio/music/TrueBlueSky.mp3", true);

	//Create Models ----------------------------------------------------------------------------------------------------------------------------------------------------	
	EntityManager entityManager;

	//Cubemap
	entityManager.addCubemapEntity();
	
	//Ground
	auto ground = entityManager.addTerrainEntity(512, 1, "res/maps/Heightmap_512.bmp", "res/textures/Grass.jpg", "res/textures/Dirt.jpg", "res/maps/Blendmap_512.jpg", "res/shader/ground_vs.glsl", "res/shader/ground_fs.glsl");

	//House
	auto house = entityManager.addOBJEntity("res/obj/houses/Farmhouse.obj", "res/textures/models/Farmhouse.jpg", "res/shader/standard_vs.glsl", "res/shader/standard_fs.glsl");
	house->getModel()->translate(glm::vec3(440, -1.5, 140));
	house->getModel()->scale(glm::vec3(2.25f, 2.25f, 2.25f));
	house->getModel()->rotate(170.0f, glm::vec3(0, 1, 0));
	house->getModel()->rotate(-5.0f, glm::vec3(1, 0, 0));
	house->getModel()->rotate(-3.0f, glm::vec3(0, 0, 1));

	//Wood
	auto wood = entityManager.addOBJEntity("res/obj/vegetation/Wood.obj", "res/textures/models/Wood.jpg", "res/shader/standard_vs.glsl", "res/shader/standard_fs.glsl");
	wood->getModel()->translate(glm::vec3(412, -3.0, 116));
	wood->getModel()->scale(glm::vec3(2.5f, 2.5f, 2.5f));

	//Axe
	auto axe = entityManager.addOBJEntity("res/obj/tools/Axe.obj", "res/textures/models/Axe.jpg", "res/shader/standard_vs.glsl", "res/shader/standard_fs.glsl");
	axe->getModel()->translate(glm::vec3(408, 3.5, 114));
	axe->getModel()->scale(glm::vec3(0.4f, 0.4f, 0.4f));
	axe->getModel()->rotate(120.0f, glm::vec3(0, -1, 0));
	axe->getModel()->rotate(120.0f, glm::vec3(0, 0, -1));	
	//------------------------------------------------------------------------------------------------------------------------------------------------------------------	
	//Renderer
	//Renderer renderer;	

	//Modelmanager
	//ModelManager modelManager(&displayManager, &audioManager);	

	//Mousepicker
	//MousePicker mousePicker(modelManager.getTerrain());

	while (!displayManager.WindowShouldClose())
	{
		//Measure Frametime
		displayManager.measureFrameTime();

		//Clear Buffer and prepare for rendering
		//renderer.prepare();
		
		//Start GUI-Frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		//Check Keyboard and Mouseinputs
		displayManager.checkForInput();

		//Render models
		entityManager.render();
		
		//Update MouseRay
		//mousePicker.update();

		////Update RenderRay Position		
		//if(!fixedRay)
		//{
		//	glm::vec3 camPos = glm::vec3(_camera->Position.x, _camera->Position.y - 1, _camera->Position.z);
		//	modelManager.createRay(camPos, _camera->Front, _camera->Yaw, renderRay, 1000.0f, 0.1f);
		//}		
		//
		////Activate Terraineditor
		//if(terrainEditor)
		//{
		//	mousePicker.calculateTerrainEntry(_camera->Position, _camera->Front);
		//	modelManager.colorPickedVertices(mousePicker._mouseRayTerrainEntry);
		//	deleteLastColoredVert = true;
		//	
		//	if(mouseIsHold)
		//	{
		//		if(raise == true && sink == false && ignoreNextClick == false)
		//		{
		//			modelManager.raiseTerrain();
		//		}
		//		else if(sink == true && raise == false && ignoreNextClick == false)
		//		{
		//			modelManager.sinkTerrain();
		//		}				
		//	}
		//}
		//else
		//{
		//	raise = false;
		//	sink = false;
		//}

		////Otherwise the last picked Vertice would remain white
		//if(terrainEditor == false && deleteLastColoredVert == true)
		//{
		//	modelManager.deleteLastColoredVertice();
		//	deleteLastColoredVert = false;
		//}		
		
		//Render Stuff		
		//renderer.render(modelManager.Models);

		//GUI Stuff
		{
			ImGui::Begin("General stuff");
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::Text("---------------------------------------------");
			ImGui::Text("Camera-Coords: X: %f, Y: %f, Z: %f", _camera->Position.x, _camera->Position.y, _camera->Position.z);
			ImGui::Text("Camera-Yaw: %f, Camera-Pitch: %f", _camera->Yaw, _camera->Pitch);
			ImGui::Text("Camera-Front: X: %f, Y: %f, Z: %f", _camera->Front.x, _camera->Front.y, _camera->Front.z);
			ImGui::Text("---------------------------------------------");
			/*ImGui::Text("Player-Coords: X: %f, Y: %f, Z: %f", displayManager._player->_playerPosition.x, displayManager._player->_playerPosition.y, displayManager._player->_playerPosition.z);
			ImGui::Text("Player-Rotation: %f", displayManager._player->_yaw);
			ImGui::Text("---------------------------------------------");
			ImGui::Checkbox("Mouse-Ray rendern", &renderRay); ImGui::SameLine(); ImGui::Checkbox("Mouse-Ray fixieren", &fixedRay);
			ImGui::Text("Mouse-Coords: X: %f, Y: %f", rawMouse_X, rawMouse_Y);
			ImGui::Text("Mouse-Ray-Direction: X: %f, Y: %f, Z: %f", mousePicker._mouseRay.x, mousePicker._mouseRay.y, mousePicker._mouseRay.z);
			ImGui::Text("---------------------------------------------");
			ImGui::Checkbox("Terrain-Editor", &terrainEditor); ImGui::SameLine(); ImGui::Checkbox("Raise", &raise); ImGui::SameLine(); ImGui::Checkbox("Sink", &sink);
			ImGui::Text("Terrain-Entry-Point: X: %f, Y: %f, Z: %f", mousePicker._mouseRayTerrainEntry.x, mousePicker._mouseRayTerrainEntry.y, mousePicker._mouseRayTerrainEntry.z);
			ImGui::Text("---------------------------------------------");*/
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