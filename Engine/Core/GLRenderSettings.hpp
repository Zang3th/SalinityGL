#pragma once

#include "ErrorManager.hpp"

namespace Engine
{
    class GLRenderSettings
    {
        public:
            GLRenderSettings() = delete;

            static void EnableDebugging();
            static void EnableMultisample();
            static void EnableDepthtest();
            static void EnableBlending();
            static void EnableWireframe();
            static void EnableClipDistance(GLushort distance);
            static void EnableCulling();

            static void DisableMultisample();
            static void DisableDepthtest();
            static void DisableBlending();
            static void DisableWireframe();
            static void DisableClipDistance(GLushort distance);
            static void DisableCulling();

            static void SetViewport(uint32 width, uint32 height);
            static void SetDepthFunc(GLushort func);
            static void SetBlendFunc(GLushort func);
            static void SetCullFace(GLushort face);

            static void ClearBuffers();
    };
}