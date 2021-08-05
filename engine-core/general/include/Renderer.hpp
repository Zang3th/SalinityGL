#pragma once

#include "Camera.hpp"
#include <glm/glm.hpp>
#include <vector>
#include "Sprite.hpp"
#include "Model.hpp"
#include "CoreDefines.hpp"
#include "ErrorManager.hpp"

namespace Core
{
    class Renderer
    {
        private:
            Camera* _camera;
            unsigned int _drawcalls;
            unsigned int _drawnVertices;
            glm::mat4 _orthoProjection;
            glm::mat4 _perspProjection;
            std::vector<Sprite*> _spriteBuffer;
            std::vector<Model*> _modelBuffer;

        public:
            Renderer(Camera* camera);
            void Prepare();
            void Submit(Sprite* sprite);
            void Submit(Model* model);
            void Flush();
            unsigned int GetDrawcalls() const;
            unsigned int GetDrawnVertices() const;
    };
}