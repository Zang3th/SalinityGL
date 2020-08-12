#include "GameDisplayManager.hpp"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

int main()
{
	//Display-Management
	GameDisplayManager gameDisplayManager(WIDTH, HEIGHT);
	gameDisplayManager.printVersion();

	//Game Initialization
	breakout.init();

	//Setup ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark(); //Setup ImGui style	
	ImGui_ImplGlfw_InitForOpenGL(gameDisplayManager.getWindow(), true); //Setup Platform/Renderer bindings
	ImGui_ImplOpenGL3_Init("#version 330");

	while (!gameDisplayManager.WindowShouldClose())
	{
		//Measure Frametime
		gameDisplayManager.measureFrameTime();

		//Start GUI-Frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		//Poll events
		gameDisplayManager.pollEvents();

		//Clear framebuffer
		gameDisplayManager.clear();

		//Render
		breakout.render();

		//GUI Stuff
		{
			ImGui::Begin("General stuff");
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		//Update stuff
		{
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			gameDisplayManager.updateDisplay();
		}
	}

	//CleanUP Stuff
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		gameDisplayManager.closeDisplay();
	}

	return 0;
}