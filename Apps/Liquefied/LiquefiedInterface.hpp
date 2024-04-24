#pragma once

#include "Engine.hpp"

namespace Liq
{
    class LiquefiedInterface final : public Engine::Interface
    {
        private:
            ImVec2 _bufferBarSize = ImVec2((float)Engine::WindowParams::WIDTH - _sidebarWidth, 80.0f);
            ImVec2 _bufferBarPos  = ImVec2(0.0f, 0.0f);

            void AddSideBar() const;
            void AddBufferBar() const;

        public:
            LiquefiedInterface();
            void InitUI()      override;
            void AddElements() override;
    };
}