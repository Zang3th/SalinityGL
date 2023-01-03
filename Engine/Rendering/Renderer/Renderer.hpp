#pragma once

namespace Engine
{
    class Renderer
    {
        public:
            virtual ~Renderer() = 0;
            virtual void Flush() = 0;
    };
}