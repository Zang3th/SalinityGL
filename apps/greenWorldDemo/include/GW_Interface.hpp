#pragma once

#include "../../../engine-core/general/include/UserInterface.hpp"
#include "../../../engine-core/general/include/CoreDefines.hpp"
#include "../../../engine-core/general/include/Renderer.hpp"
#include "../../../engine-core/vendor/imgui/imgui_plot_var.h"
#include "../../../engine-core/general/include/Profiler.hpp"

namespace GW
{
    class Interface : public Core::UserInterface
    {
        private:
            Core::Renderer* _renderer;
            Core::Camera* _camera;
            bool _showOverlay;
            ImVec2 _overlayPos, _overlayPosPivot;
            ImGuiWindowFlags _windowFlags;

            void CalcOverlayPosition();

        public:
            Interface(Core::WindowManager* window, Core::Renderer* renderer, Core::Camera* camera);
            void AddElements() override;
    };
}