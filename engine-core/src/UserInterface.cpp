#include "UserInterface.hpp"

namespace Core
{
    // ----- Public -----

    UserInterface::UserInterface(const WindowManager* window)
        :   _window(window)
    {
        //Setup ImGui
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(_window->GetWindow(), true);
        ImGui_ImplOpenGL3_Init("#version 450");
    }

    UserInterface::~UserInterface()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void UserInterface::PrepareFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void UserInterface::Render()
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}