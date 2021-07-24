#pragma once

#include "../../../engine-core/general/include/UserInterface.hpp"
#include "../../../engine-core/general/include/CoreDefines.hpp"

class GreenWorldInterface : public Core::UserInterface
{
    public:
        GreenWorldInterface(Core::Ref<Core::WindowManager> window);
        void AddElements();
};