#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "Sprite.hpp"
#include "CoreDefines.hpp"

namespace Core
{
    class Renderer
    {
        private:
            unsigned int _drawcalls;
            unsigned int _drawnVertices;
            glm::mat4 _orthoProjection;
            std::vector<Ref<Sprite>> _spriteBuffer;

        public:
            Renderer();
            void Prepare();
            void Submit(Ref<Sprite> sprite);
            void Flush();
    };
}