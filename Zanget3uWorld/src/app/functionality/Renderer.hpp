#pragma once

#include "Basemodel.hpp"

class Renderer
{
public:
	void prepare()
	{
		GLCall(glClearColor(0.611f, 0.705f, 0.752f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}

	void render(const std::vector<Basemodel*>& Models)
	{
		for(Basemodel* m : Models)
		{		
			if(m->renderModel == true)
			{
				m->draw();
				GLCall(glDrawElements(GL_TRIANGLES, m->getNumberOfVertices(), GL_UNSIGNED_INT, nullptr));
				m->undraw();
			}
			else
			{
				//Model is not visible -> don't render it!
			}
		}
	}
};