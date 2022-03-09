#include "BaseInterface.hpp"

namespace Engine
{
    // ----- Public -----

    BaseInterface::BaseInterface(const Window* window)
        :   _window(window)
    {
        //Setup ImGui
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(_window->GetWindow(), true);
        ImGui_ImplOpenGL3_Init("#version 450");
    }

    BaseInterface::~BaseInterface()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void BaseInterface::PrepareFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void BaseInterface::Render()
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}