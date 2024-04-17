#pragma once

#include "Engine.hpp"

namespace Liq
{
    class LiquefiedInterface final : public Engine::Interface
    {
        private:
            ImGuiWindowFlags _windowFlags;
            ImVec2           _sidebarPos, _sidebarSize, _overlayPivot;
            float            _windowAlphaValue, _menuBarHeight, _sidebarWidth;
            bool             _showOverlay;

            void             CalculateSidebarDimensions();
            void             AddMenuBar();
            void             AddSideBar();

        public:
            LiquefiedInterface();
            void AddElements() final;
    };
}