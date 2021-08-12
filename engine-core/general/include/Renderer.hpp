#pragma once

#include "Camera.hpp"
#include <glm/glm.hpp>
#include <vector>
#include "Sprite.hpp"
#include "Model.hpp"
#include "CoreDefines.hpp"
#include "ErrorManager.hpp"
#include "Cubemap.hpp"

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
            std::vector<const Sprite*> _spriteBuffer;
            std::vector<const Model*> _modelBuffer;
            const Cubemap* _cubemap;

        public:
            Renderer(Camera* camera);
            void Prepare();
            void Submit(const Sprite* sprite);
            void Submit(const Model* model);
            void Submit(const Cubemap* cubemap);
            void Flush();
            unsigned int GetDrawcalls() const;
            unsigned int GetDrawnVertices() const;
    };
}