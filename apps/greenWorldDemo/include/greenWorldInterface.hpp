#pragma once

#include "../../../engine-core/general/include/UserInterface.hpp"
#include "../../../engine-core/general/include/CoreDefines.hpp"
#include "../../../engine-core/general/include/Renderer.hpp"
#include "../../../engine-core/vendor/imgui/implot.h"
#include <vector>

class GreenWorldInterface : public Core::UserInterface
{
    private:
        Core::Ref<Core::Renderer> _renderer;
        bool _showOverlay, _showFpsGraph;
        ImVec2 _overlayPos, _overlayPosPivot;
        ImGuiWindowFlags _windowFlags;
        ImPlotAxisFlags _graphFlags;
        std::vector<glm::vec2> _framesAccumulated;

        void CalcOverlayPosition();

    public:
        GreenWorldInterface(Core::Ref<Core::WindowManager> window, Core::Ref<Core::Renderer> renderer);
        ~GreenWorldInterface();
        void AddElements();
};