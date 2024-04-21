#pragma once

#include "Engine.hpp"

namespace GW
{
    class GreenWorldInterface final : public Engine::Interface
    {
        private:
            ImVec2 _shaderFieldPos   = ImVec2(0.0f, 0.0f);
            ImVec2 _shaderFieldSize  = ImVec2(0.0f, 0.0f);
            ImVec2 _texFieldPos      = ImVec2(0.0f, 0.0f);
            ImVec2 _texFieldSize     = ImVec2(0.0f, 0.0f);
            ImVec2 _resourceBarPivot = ImVec2(0.0f, 0.0f);
            float  _resourceBarWidth = 210.0f;

            void AddMenuBar() const;
            void AddSideBar() const;
            void AddResourceBar() const;
            void CalcElementSizes() override;

        public:
            GreenWorldInterface();
            void InitUI()      override;
            void AddElements() override;
    };
}