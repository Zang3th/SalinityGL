#pragma once

#include <memory>
#include <cstdint>

namespace Engine
{
    // --- Smart-Pointer stuff
    template<typename T>
    using Scope = std::unique_ptr<T>;

    template<typename T, typename ... Args>
    constexpr Scope<T> MakeScope(Args&&... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using Ref = std::shared_ptr<T>;

    template<typename T, typename ... Args>
    constexpr Ref<T> MakeRef(Args&&... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    // --- Renaming stuff
    typedef int8_t   int8;
    typedef int16_t  int16;
    typedef int32_t  int32;
    typedef int64_t  int64;
    typedef uint8_t  uint8;
    typedef uint16_t uint16;
    typedef uint32_t uint32;
    typedef uint64_t uint64;
}