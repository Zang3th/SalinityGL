#pragma once

#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "RawData.hpp"

class Model
{
private:
	VertexArray* _vao = nullptr;
	VertexBuffer* _vbo = nullptr;
	IndexBuffer* _ib = nullptr;
	Shader* _shader = nullptr;
	Texture* _texture = nullptr;
	RawData* _data = nullptr;

public:
	Model(RawData* dataToUse, Shader* shaderToUse)
		: _data(dataToUse), _shader(shaderToUse)
	{
		this->initialize();
	}

	~Model()
	{
		delete _vao;
		delete _vbo;
		delete _ib;
		delete _texture;
	}

	void initialize()
	{
		//Erstellt und bindet VAO
		_vao = new VertexArray();
		_vao->bind();

		//Load Texture
		_texture = new Texture("res/textures/Brick.jpg");
		_texture->bind(0); //Textureslot

		//Erstellt VBO und konfiguriert VAO
		_vbo = new VertexBuffer(_data->_verticeData, _data->_verticeSize);
		_vao->DefineAttributes(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); //Position attribute
		_vao->DefineAttributes(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))); //Texture attribute
		
		//Erstellt IB
		_ib = new IndexBuffer(_data->_indiceData, _data->_indiceSize);

		//Unbindet VAO und VBO
		_vbo->unbind();
		_vao->unbind();
	}

	void draw()
	{
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 projection = glm::perspective(glm::radians(_camera->Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f);
		glm::mat4 view = _camera->GetViewMatrix();
		_shader->bind();
		_shader->SetUniformMat4f("model", model);
		_shader->SetUniformMat4f("projection", projection);
		_shader->SetUniformMat4f("view", view);
		_vao->bind();
	}

	void undraw()
	{
		_shader->unbind();
		_vao->unbind();
	}
};