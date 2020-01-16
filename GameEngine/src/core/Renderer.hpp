#pragma once

#include "Model.hpp"

class Renderer
{
public:
	void prepare()
	{
		GLCall(glClearColor(0.070f, 0.098f, 0.427f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}

	void render(Model* model)
	{
		model->draw();
		GLCall(glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr));
		model->undraw();
	}
};