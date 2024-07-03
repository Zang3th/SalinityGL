#pragma once

#include "Engine.hpp"

namespace CS
{
    class CellSimInterface final : public Engine::Interface
    {
        private:
            void AddMenuBar() const;
            void AddSideBar();
            void CheckCellBoundaries();

        public:
            CellSimInterface();
            void Init()        override;
            void AddElements() override;
    };
}
