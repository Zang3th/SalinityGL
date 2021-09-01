#pragma once

#include "BaseInterface.hpp"
#include "CoreDefines.hpp"
#include "Renderer.hpp"
#include "imgui_plot_var.h"
#include "Profiler.hpp"
#include "ResourceManager.hpp"

namespace GW
{
    class Interface : public Core::BaseInterface
    {
        private:
            ImVec2                  _sidebarPos, _shaderFieldPos, _texFieldPos;
            ImVec2                  _overlaySize, _bottomFieldSizes, _overlayPivot;
            const Core::Renderer*   _renderer;
            const Core::Camera*     _camera;
            ImGuiWindowFlags        _windowFlags;
            float                   _windowAlphaValue;
            bool                    _showOverlay;

            static void SetDarkThemeColors();
            static void TextCentered(const char*);
            void SetOverlayParameters();

        public:
            Interface(const Core::Window* window, const Core::Renderer* renderer, const Core::Camera* camera);
            void AddElements() final;
    };
}