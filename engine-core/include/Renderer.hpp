#pragma once

#include "Camera.hpp"
#include "glm.hpp"
#include "Sprite.hpp"
#include "Model.hpp"
#include "CoreDefines.hpp"
#include "ErrorManager.hpp"
#include "Cubemap.hpp"

#include <vector>

namespace Core
{
    class Renderer
    {
        private:
            std::vector<const Sprite*>  _spriteBuffer;
            std::vector<const Model*>   _modelBuffer;
            glm::mat4                   _orthoProjection;
            glm::mat4                   _perspProjection;
            Camera*                     _camera;
            const Cubemap*              _cubemap;
            unsigned int                _drawcalls;
            unsigned int                _drawnVertices;

        public:
            explicit Renderer(Camera* camera);
            void PrepareFrame();
            void Submit(const Sprite* sprite);
            void Submit(const Model* model);
            void Submit(const Cubemap* cubemap);
            void Flush();
            [[nodiscard]] unsigned int GetDrawcalls() const;
            [[nodiscard]] unsigned int GetDrawnVertices() const;
    };
}