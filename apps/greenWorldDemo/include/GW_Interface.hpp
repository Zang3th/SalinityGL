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
            ImVec2                  _overlayPos, _overlayPosPivot;
            const Core::Renderer*   _renderer;
            const Core::Camera*     _camera;
            ImGuiWindowFlags        _windowFlags;
            bool                    _showOverlay;

            void CalcOverlayPosition();

        public:
            Interface(const Core::WindowManager* window, const Core::Renderer* renderer, const Core::Camera* camera);
            void AddElements() override;
    };
}