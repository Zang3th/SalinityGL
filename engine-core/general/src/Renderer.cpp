#include "Renderer.hpp"

namespace Core
{
    // ----- Public -----

    Renderer::Renderer()
        : _drawcalls(0), _drawnVertices(0)
    {
        _orthoProjection = glm::ortho(0.0f, (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT, 0.0f, -1.0f, 1.0f);
    }

    void Renderer::Prepare()
    {
        //Reset render stats for current frame
        _drawcalls = 0;
        _drawnVertices = 0;

        //Clear sprite buffer
        _spriteBuffer.clear();
    }

    void Renderer::Submit(Ref<Sprite> sprite)
    {
        _spriteBuffer.push_back(sprite);
    }

    void Renderer::Flush()
    {
        for(auto sprite : _spriteBuffer)
        {
            _drawnVertices += sprite->Draw(_orthoProjection);
            _drawcalls++;
        }
    }
}