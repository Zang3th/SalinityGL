#pragma once

#include "Model.hpp"

class Renderer
{
public:
	void render(Model* model)
	{
		model->draw();
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
	}
};