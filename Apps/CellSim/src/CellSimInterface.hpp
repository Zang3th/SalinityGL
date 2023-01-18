#pragma once

#include "Engine.hpp"

namespace CS
{
    class CellSimInterface final : public Engine::Interface
    {
        private:
            ImVec2           _sidebarPos, _sidebarSize;
            ImVec2           _overlayPivot = ImVec2(1.0f, 0.0f);
            ImGuiWindowFlags _windowFlags;
            float            _windowAlphaValue;
            bool             _showOverlay;

            void             SetOverlayParameters();

        public:
            CellSimInterface();
            void AddElements() final;
    };
}