#include "GLRenderSettings.hpp"

namespace Engine
{
    // ----- Public -----

    void GLRenderSettings::EnableDebugging()
    {
        //Enable extensive debugging information (available since OpenGL 4.3)
        GLCall(glEnable(GL_DEBUG_OUTPUT));
        GLCall(glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS));
        GLCall(glDebugMessageCallback(ErrorManager::OpenGLMessageCallback, nullptr));
        GLCall(glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE));
    }

    void GLRenderSettings::EnableMultisample()
    {
        GLCall(glEnable(GL_MULTISAMPLE));
    }

    void GLRenderSettings::EnableDepthtest()
    {
        GLCall(glEnable(GL_DEPTH_TEST));
    }

    void GLRenderSettings::EnableBlending()
    {
        GLCall(glEnable(GL_BLEND));
    }

    void GLRenderSettings::EnableWireframe()
    {
        GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
    }

    void GLRenderSettings::EnableClipDistance(GLushort distance)
    {
        GLCall(glEnable(distance));
    }

    void GLRenderSettings::EnableCulling()
    {
        GLCall(glEnable(GL_CULL_FACE));
    }

    void GLRenderSettings::DisableMultisample()
    {
        GLCall(glDisable(GL_MULTISAMPLE));
    }

    void GLRenderSettings::DisableDepthtest()
    {
        GLCall(glDisable(GL_DEPTH_TEST));
    }

    void GLRenderSettings::DisableBlending()
    {
        GLCall(glDisable(GL_BLEND));
    }

    void GLRenderSettings::DisableWireframe()
    {
        GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
    }

    void GLRenderSettings::DisableClipDistance(GLushort distance)
    {
        GLCall(glDisable(distance));
    }

    void GLRenderSettings::DisableCulling()
    {
        GLCall(glDisable(GL_CULL_FACE));
    }

    void GLRenderSettings::SetViewport(uint32 width, uint32 height)
    {
        GLCall(glViewport(0, 0, width, height));
    }

    void GLRenderSettings::SetDepthFunc(GLushort func)
    {
        GLCall(glDepthFunc(func));
    }

    void GLRenderSettings::SetBlendFunc(GLushort func)
    {
        GLCall(glBlendFunc(GL_SRC_ALPHA, func));
    }

    void GLRenderSettings::SetCullFace(GLushort face)
    {
        GLCall(glCullFace(face));
    }

    void GLRenderSettings::ClearBuffers()
    {
        GLCall(glClearColor(0.0, 0.0, 0.0, 1.0));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    }
}