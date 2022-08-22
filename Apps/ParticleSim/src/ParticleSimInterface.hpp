#pragma once

#include "Engine.hpp"

namespace ParticleSim
{
    class Interface : public Engine::BaseInterface
    {
        private:
            ImVec2           _sidebarPos, _sidebarSize;
            ImVec2           _overlayPivot = ImVec2(1.0f, 0.0f);
            ImGuiWindowFlags _windowFlags;
            float            _windowAlphaValue;
            bool             _showOverlay;

            void             SetOverlayParameters();

        public:
            Interface();
            void AddElements() final;
    };
}