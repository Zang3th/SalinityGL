#pragma once

#include "../../../engine-core/general/include/UserInterface.hpp"
#include "../../../engine-core/general/include/CoreDefines.hpp"
#include "../../../engine-core/general/include/Renderer.hpp"
#include "../../../engine-core/vendor/imgui/imgui_plot_var.h"
#include "../../../engine-core/general/include/Profiler.hpp"

class GreenWorldInterface : public Core::UserInterface
{
    private:
        Core::Ref<Core::Renderer> _renderer;
        bool _showOverlay;
        ImVec2 _overlayPos, _overlayPosPivot;
        ImGuiWindowFlags _windowFlags;

        void CalcOverlayPosition();

    public:
        GreenWorldInterface(Core::Ref<Core::WindowManager> window, Core::Ref<Core::Renderer> renderer);
        void AddElements() override;
};