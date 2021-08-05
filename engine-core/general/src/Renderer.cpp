#include "Renderer.hpp"

namespace Core
{
    // ----- Public -----

    Renderer::Renderer()
    : _drawcalls(0), _drawnVertices(0), _orthoProjection(glm::ortho(0.0f, (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT, 0.0f, -1.0f, 1.0f))
    {

    }

    void Renderer::Prepare()
    {
        //Reset render stats for current frame
        _drawcalls = 0;
        _drawnVertices = 0;

        //Clear sprite buffer
        _spriteBuffer.clear();
    }

    void Renderer::Submit(Sprite* sprite)
    {
        _spriteBuffer.push_back(sprite);
    }

    void Renderer::Flush()
    {
        if(WireframeRendering){
            GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));}
        else{
            GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));}

        for(const auto& sprite : _spriteBuffer)
        {
            _drawnVertices += sprite->Draw(_orthoProjection);
            _drawcalls++;
        }
    }

    unsigned int Renderer::GetDrawcalls() const
    {
        return _drawcalls;
    }

    unsigned int Renderer::GetDrawnVertices() const
    {
        return _drawnVertices;
    }
}