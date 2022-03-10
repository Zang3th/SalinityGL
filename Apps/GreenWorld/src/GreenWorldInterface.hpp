#pragma once

#include "Engine.hpp"

namespace GreenWorld
{
    class Interface : public Engine::BaseInterface
    {
        private:
            ImVec2           _sidebarPos,  _shaderFieldPos,   _texFieldPos;
            ImVec2           _overlaySize, _bottomFieldSizes, _overlayPivot;
            ImGuiWindowFlags _windowFlags;
            float            _windowAlphaValue;
            bool             _showOverlay;

            static void SetDarkThemeColors();
            static void TextCentered(const char* text);
            void        SetOverlayParameters();

        public:
            Interface();
            void     AddElements() final;
    };
}