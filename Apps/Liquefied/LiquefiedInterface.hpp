#pragma once

#include "Engine.hpp"

namespace Liq
{
    class LiquefiedInterface final : public Engine::Interface
    {
        private:
            void AddMenuBar() const;
            void AddSideBar() const;

        public:
            LiquefiedInterface();
            void InitUI()      override;
            void AddElements() override;
    };
}