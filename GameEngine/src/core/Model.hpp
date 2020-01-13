#pragma once

#include "VertexArray.hpp"
#include "VertexBuffer.hpp"

class Model
{
private:
	void* _vertices = nullptr;
	int _size = 0;
	VertexArray* _vao;
	VertexBuffer* _vbo;

public:
	Model(void* data, int size)
		: _vertices(data), _size(size)
	{
		//Erstellt und bindet VAO
		_vao = new VertexArray();
		_vao->bind();

		//Erstellt VBO und konfiguriert VAO
		_vbo = new VertexBuffer(_vertices, _size);
		_vao->DefineAttributes(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

		//Unbindet VAO und VBO
		_vbo->unbind();
		_vao->unbind();
	}

	~Model()
	{
		delete _vao;
		delete _vbo;
	}

	void draw()
	{
		_vao->bind();
	}
};
