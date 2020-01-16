#pragma once

#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

class Model
{
private:
	void* _vertices = nullptr;
	void* _indices = nullptr;
	int _vertice_size = 0;
	int _indice_size = 0;
	VertexArray* _vao = nullptr;
	VertexBuffer* _vbo = nullptr;
	IndexBuffer* _ib = nullptr;
	Shader* _shader = nullptr;
	Texture* _texture = nullptr;

public:
	Model(void* vertice_data, int vertice_size, void* indice_data, int indice_size, Shader* shaderToUse)
		: _vertices(vertice_data), _vertice_size(vertice_size), _indices(indice_data), _indice_size(indice_size), _shader(shaderToUse)
	{
		this->initialize();
	}

	~Model()
	{
		delete _vao;
		delete _vbo;
		delete _ib;
	}

	void initialize()
	{		
		//Erstellt und bindet VAO
		_vao = new VertexArray();
		_vao->bind();

		//Load Texture
		_texture = new Texture("res/textures/Stone.jpg");
		_texture->bind(0); //Textureslot

		//Erstellt VBO und konfiguriert VAO
		_vbo = new VertexBuffer(_vertices, _vertice_size);
		_vao->DefineAttributes(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); //Position attribute
		_vao->DefineAttributes(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))); //Texture attribute
		
		//Erstellt IB
		_ib = new IndexBuffer(_indices, _indice_size);

		//Unbindet VAO und VBO
		_vbo->unbind();
		_vao->unbind();
	}

	void draw()
	{
		_shader->bind();
		_vao->bind();
	}

	void undraw()
	{
		_shader->unbind();
		_vao->unbind();
	}
};