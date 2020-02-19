#pragma once

#include "OpenGLErrorManager.hpp"

class RenderStateManager
{
public:
	void setNormalRenderState()
	{
		GLCall(glDisable(GL_BLEND));
		GLCall(glEnable(GL_DEPTH_TEST));
	}

	void setTransparencyRenderState()
	{
		GLCall(glDisable(GL_DEPTH_TEST));
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	}

	void deactivateDepthMask()
	{
		GLCall(glDepthMask(GL_FALSE));
	}

	void activateDepthMask()
	{
		GLCall(glDepthMask(GL_TRUE));
	}
};