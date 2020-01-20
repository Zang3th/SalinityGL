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
	VertexBuffer* _vbo1 = nullptr;
	VertexBuffer* _vbo2 = nullptr;
	IndexBuffer* _ib = nullptr;
	Shader* _shader = nullptr;
	RawData* _data = nullptr;
	glm::mat4 _model = glm::mat4(1.0f);
	glm::mat4 _projection;
	glm::mat4 _view;
	unsigned int _texSlot;

public:
	Model(RawData* dataToUse, Shader* shaderToUse,  unsigned int textureSlot)
		: _data(dataToUse), _shader(shaderToUse), _texSlot(textureSlot)
	{
		this->initialize();
	}

	~Model()
	{
		delete _vao;
		delete _vbo1;
		delete _vbo2;
		delete _ib;
	}

	void initialize()
	{
		//Erstellt und bindet VAO
		_vao = new VertexArray();
		_vao->bind();
		
		//Erstellt VBO und konfiguriert VAO
		_vbo1 = new VertexBuffer(&_data->_vertices[0], _data->_verticeSize);
		_vao->DefineAttributes(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); //Position attribute
		_vbo2 = new VertexBuffer(&_data->_texCoords[0], _data->_texCoordSize);
		_vao->DefineAttributes(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0); //Texture attribute
				
		//Erstellt IB
		_ib = new IndexBuffer(&_data->_indices[0], _data->_indiceSize);

		//Unbindet VAO und VBO
		_vbo1->unbind();
		_vbo2->unbind();
		_vao->unbind();
	}

	void draw()
	{
		_projection = glm::perspective(glm::radians(_camera->Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f);
		_view = _camera->GetViewMatrix();
		_shader->bind();
		_shader->SetUniformMat4f("model", _model);
		_shader->SetUniformMat4f("projection", _projection);
		_shader->SetUniformMat4f("view", _view);
		_shader->SetUniform1i("textureSampler", _texSlot);
		_vao->bind();
	}

	void undraw()
	{
		_shader->unbind();
		_vao->unbind();		
	}

	void translate(const glm::vec3& position)
	{
		_model = glm::translate(_model, position);
	}

	void rotate(const float& angle, const glm::vec3& axis)
	{
		_model = glm::rotate(_model, glm::radians(angle), axis); 
	}

	void scale(const glm::vec3& scalar)
	{
		_model = glm::scale(_model, scalar);
	}

	unsigned int getNumberOfVertices()
	{
		return _data->_verticesToRender;
	}
};