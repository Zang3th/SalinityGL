#pragma once

#include "Engine.hpp"

namespace Liq
{
    class LiquefiedInterface final : public Engine::Interface
    {
        private:
            float  _bufferBarHeight  = 80.0f;
            ImVec2 _bufferBarSize    = ImVec2((float)Engine::WindowParams::WIDTH - _sidebarWidth, _bufferBarHeight);
            ImVec2 _bufferBarPos     = ImVec2(0.0f, 0.0f);
            float  _debugWindowAlpha = 0.6f;
            float  _debugWindowWidth = 250.0f;
            ImVec2 _debugWindowPos   = ImVec2((float)Engine::WindowParams::WIDTH - _sidebarWidth, _bufferBarHeight);
            ImVec2 _debugWindowSize  = ImVec2(_debugWindowWidth,(float)Engine::WindowParams::HEIGHT);

            void AddSideBar() const;
            void AddVerticalBarDivider(float xpos) const;
            void AddBufferBar() const;

        public:
            LiquefiedInterface();
            void Init()        override;
            void AddElements() override;
    };
}
