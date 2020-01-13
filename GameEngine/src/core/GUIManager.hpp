#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

class GUIManager
{
public:
	GUIManager()
	{
		
	}

	void initializeGUI(GLFWwindow* window)
	{
		//Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		//Setup Dear ImGui style
		ImGui::StyleColorsDark();

		//Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 130");
	}

	void startFrame()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void newWindow(const char* title)
	{
		ImGui::Begin(title);
	}

	void printFPS()
	{
		ImGui::Text("\nApplication average %.3f ms/frame (%.1f FPS)", 1000.f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

	void exitWindow()
	{
		ImGui::End();
	}

	void renderGUI()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void cleanUPGUI()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
};