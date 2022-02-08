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

    //Datatypes
    typedef int8_t   int8;
    typedef int16_t  int16;
    typedef int32_t  int32;
    typedef uint8_t  uint8;
    typedef uint16_t uint16;
    typedef uint32_t uint32;

    //Extern settings
    extern const uint32 WINDOW_WIDTH;
    extern const uint32 WINDOW_HEIGHT;
    extern const uint32 PLANE_SIZE;
    extern       bool   WIREFRAME_RENDERING;
    extern       bool   DEBUG_SPRITES;
}