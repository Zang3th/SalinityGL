#include "Simulation.hpp"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

int main()
{
	//Create application
	Simulation simulation;
	simulation.printVersion();
	
	//Application initialization
	simulation.init();

	//Setup ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark(); //Setup ImGui-style	
	ImGui_ImplGlfw_InitForOpenGL(simulation.getWindow(), true); //Setup Platform/Renderer bindings
	ImGui_ImplOpenGL3_Init("#version 330");

	while (!simulation.windowShouldClose())
	{
		//Measure frametime
		simulation.measureFrameTime();

		//Start GUI-Frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		//Check for input
		simulation.processInput();
		
		//Clear buffers
		simulation.clear();

		//Update
		simulation.updateModels();

		//Render
		simulation.render();

		//GUI Stuff
		{
			ImGui::Begin("General stuff");
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::Text("---------------------------------------------");
			ImGui::Text("Camera-Coords: X: %f, Y: %f, Z: %f", camera.Position.x, camera.Position.y, camera.Position.z); 
			ImGui::Text("Camera-Yaw: %f, Camera-Pitch: %f", camera.Yaw, camera.Pitch);
			ImGui::Text("Camera-Front: X: %f, Y: %f, Z: %f", camera.Front.x, camera.Front.y, camera.Front.z);
			ImGui::Text("---------------------------------------------");
			ImGui::Text("Rendered Vertices: %d", VERTICES_TO_RENDER);			
			ImGui::End();
		}

		//Update stuff
		{
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			simulation.updateDisplay();
		}
	}

	//CleanUP Stuff
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		simulation.closeDisplay();
	}

	return 0;
}