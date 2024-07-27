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
    using int8   = int8_t;
    using int16  = int16_t;
    using int32  = int32_t;
    using int64  = int64_t;
    using uint8  = uint8_t;
    using uint16 = uint16_t;
    using uint32 = uint32_t;
    using uint64 = uint64_t;

    #define COLOR_WHITE glm::vec3(1.0f, 1.0f, 1.0f)
    #define COLOR_BLACK glm::vec3(1.0f, 1.0f, 1.0f)
    #define COLOR_RED   glm::vec3(1.0f, 0.0f, 0.0f)
    #define COLOR_GREEN glm::vec3(0.0f, 1.0f, 0.0f)
    #define COLOR_BLUE  glm::vec3(0.0f, 0.0f, 1.0f)

    struct LogFloatXY
    {
        float  val;
        uint32 x;
        uint32 y;
    };

    struct ColorEntry
    {
        float scalar;
        float r;
        float g;
        float b;
    };

    struct PxColor
    {
        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char a;
    };

    struct GridPos
    {
        uint32 x;
        uint32 y;

        GridPos(uint32 xpos, uint32 ypos)
            : x(xpos), y(ypos)
        {}

        inline bool operator==(const GridPos& comp) const
        {
            return (x == comp.x) && (y == comp.y);
        }
    };
}
