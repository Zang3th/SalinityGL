#pragma once

#include "Engine.hpp"

namespace Liq
{
    class LiquefiedInterface final : public Engine::Interface
    {
        private:
            ImVec2 _bufferBarSize = ImVec2((float)Engine::WindowParams::WIDTH - _sidebarWidth, 80.0f - _menuBarHeight);
            ImVec2 _bufferBarPos  = ImVec2(0.0f, _menuBarHeight);

            void AddMenuBar() const;
            void AddSideBar() const;
            void AddBufferBar() const;

        public:
            LiquefiedInterface();
            void InitUI()      override;
            void AddElements() override;
    };
}