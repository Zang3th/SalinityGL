#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "Sprite.hpp"
#include "CoreDefines.hpp"
#include "ErrorManager.hpp"

namespace Core
{
    class Renderer
    {
        private:
            unsigned int _drawcalls;
            unsigned int _drawnVertices;
            glm::mat4 _orthoProjection;
            std::vector<Sprite*> _spriteBuffer;

        public:
            Renderer();
            void Prepare();
            void Submit(Sprite* sprite);
            void Flush();
            unsigned int GetDrawcalls() const;
            unsigned int GetDrawnVertices() const;
    };
}