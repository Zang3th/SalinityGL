#pragma once

#include "Engine.hpp"

namespace GW
{
    class GreenWorldInterface final : public Engine::Interface
    {
        private:
            ImGuiWindowFlags _windowFlags;
            ImVec2           _sidebarPos, _sidebarSize;
            ImVec2           _shaderFieldPos, _shaderFieldSize, _texFieldPos, _texFieldSize;
            ImVec2           _overlayPivot, _shaderPivot, _texturePivot;
            float            _windowAlphaValue, _menuBarHeight, _sidebarWidth, _shaderTexBarWidth;
            bool             _showOverlay;

            void             CalculateSidebarDimensions();

        public:
            GreenWorldInterface();
            void AddElements() final;
    };
}