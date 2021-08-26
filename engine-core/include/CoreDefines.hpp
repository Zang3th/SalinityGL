#pragma once

#include <memory>
#include <cstdint>

namespace Core
{
    //Smart-Pointer-Stuff
    template<typename T>
        using Scope = std::unique_ptr<T>;

    template<typename T, typename ... Args>
    constexpr Scope<T> MakeScope(Args&& ... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T>
        using Ref = std::shared_ptr<T>;

    template<typename T, typename ... Args>
    constexpr Ref<T> MakeRef(Args&& ... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    //Global defines
    #define WINDOW_WIDTH    1920
    #define WINDOW_HEIGHT   1080
    #define PLANE_SIZE      128

    //Datatypes
    typedef int8_t int8;
    typedef int16_t int16;
    typedef int32_t int32;
    typedef uint8_t uint8;
    typedef uint16_t uint16;
    typedef uint32_t uint32;

    //Extern settings
    extern bool WireframeRendering;    
}