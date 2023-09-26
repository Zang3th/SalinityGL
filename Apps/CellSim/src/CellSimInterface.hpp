#pragma once

#include "Engine.hpp"

namespace CS
{
    class CellSimInterface final : public Engine::Interface
    {
        private:
            ImGuiWindowFlags _windowFlags;
            ImVec2           _sidebarPos, _sidebarSize, _overlayPivot;
            float            _windowAlphaValue, _menuBarHeight, _sidebarWidth;
            bool             _showOverlay;

            void             CalculateSidebarDimensions();
            void             AddMenuBar();
            void             CheckCellBoundaries();
            void             AddSideBar();

        public:
            CellSimInterface();
            void AddElements() final;
    };
}