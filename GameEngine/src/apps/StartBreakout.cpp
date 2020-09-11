////#define DEBUG
//
//#include "GameDisplayManager.hpp"
//
//#ifdef DEBUG
//	#include <imgui/imgui.h>
//	#include <imgui/imgui_impl_glfw.h>
//	#include <imgui/imgui_impl_opengl3.h>
//#endif
//
//int main()
//{	
//	//Display-Management
//	GameDisplayManager gameDisplayManager(WIDTH, HEIGHT);
//	gameDisplayManager.printVersion();
//		
//	//Game Initialization
//	breakout.init();
//
//	//Setup ImGui
//	#ifdef DEBUG
//		IMGUI_CHECKVERSION();
//		ImGui::CreateContext();
//		ImGui::StyleColorsDark(); //Setup ImGui style	
//		ImGui_ImplGlfw_InitForOpenGL(gameDisplayManager.getWindow(), true); //Setup Platform/Renderer bindings
//		ImGui_ImplOpenGL3_Init("#version 330");
//	#endif
//	
//	while (!gameDisplayManager.WindowShouldClose())
//	{
//		//Measure Frametime
//		gameDisplayManager.measureFrameTime();
//		
//		#ifdef DEBUG
//			//Start GUI-Frame
//			ImGui_ImplOpenGL3_NewFrame();
//			ImGui_ImplGlfw_NewFrame();
//			ImGui::NewFrame();
//		#endif
//		
//		//Poll events
//		gameDisplayManager.pollEvents();
//
//		//Clear framebuffer
//		gameDisplayManager.clear();
//
//		//Manage input
//		breakout.processInput(deltaTime);
//
//		//Update
//		breakout.update(deltaTime);
//		
//		//Render
//		breakout.render();
//
//		//GUI Stuff
//		#ifdef DEBUG
//		{
//			ImGui::Begin("General stuff");
//			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
//			ImGui::Text("Active PowerUps:");
//			ImGui::Text("Speed: %d", ACTIVE_SPEED_EFFECTS);
//			ImGui::Text("Sticky: %d", ACTIVE_STICKY_EFFECTS);
//			ImGui::Text("PassThrough: %d", ACTIVE_PASSTHROUGH_EFFECTS);
//			ImGui::Text("PadIncrease: %d", ACTIVE_PADINREASE_EFFECTS);
//			ImGui::End();
//		}
//		#endif
//		
//		//Update stuff
//		{
//			#ifdef DEBUG
//				ImGui::Render();
//				ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//			#endif
//			gameDisplayManager.updateDisplay();
//		}
//	}
//
//	//CleanUP Stuff
//	{
//		#ifdef DEBUG
//			ImGui_ImplOpenGL3_Shutdown();
//			ImGui_ImplGlfw_Shutdown();
//			ImGui::DestroyContext();
//		#endif
//		gameDisplayManager.closeDisplay();
//	}
//
//	return 0;
//}