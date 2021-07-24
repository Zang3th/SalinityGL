#include "greenWorldInterface.hpp"

GreenWorldInterface::GreenWorldInterface(Core::Ref<Core::WindowManager> window)
    : Core::UserInterface(window)
{

}

void GreenWorldInterface::AddElements()
{
    ImGui::ShowDemoWindow();
}