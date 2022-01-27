#include "WaterRenderer.hpp"

namespace Core
{
    // ----- Public -----

    WaterRenderer::WaterRenderer()
    {
        _reflectFBO = MakeScope<FrameBuffer>();
        _reflectFBO->Bind();
        _reflectFBO->SetColorAttachment0();
        _reflectFBO->CreateTextureAttachment(200, 200);
        _reflectFBO->CreateDepthBufferAttachment(200, 200);
        _reflectFBO->Unbind();
    }

    void WaterRenderer::StartFrame()
    {
        _reflectFBO->Bind(200, 200);
    }

    void WaterRenderer::EndFrame()
    {
        _reflectFBO->Unbind();
    }

    Texture* WaterRenderer::GetReflectTexture() const
    {
        return _reflectFBO->GetTexture();
    }

    Texture* WaterRenderer::GetRefractTexture() const
    {
        return _refractFBO->GetTexture();
    }

    Texture* WaterRenderer::GetRefractDepthTexture() const
    {
        return _refractFBO->GetDepthTexture();
    }
}