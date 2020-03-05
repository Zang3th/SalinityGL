#pragma once

#include "Basemodel.hpp"
#include "RawData.hpp"

class Primitivemodel : public Basemodel
{
private:
	Shader* _shader = nullptr;
	RawData* _data = nullptr;

public:
	Primitivemodel(RawData* dataToUse, Shader* shaderToUse)
		: _data(dataToUse), _shader(shaderToUse)
	{
		this->initialize();
	}

	~Primitivemodel()
	{
		delete _vao;
		delete _vbo1;
		delete _ib;
	}

	void initialize() override
	{
		//Erstellt und bindet VAO
		_vao = new VertexArray();
		_vao->bind();

		//Erstellt VBO und konfiguriert VAO
		_vbo1 = new VertexBuffer(&_data->_vertices[0], _data->_verticeSize, false);
		_vao->DefineAttributes(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); //Position attribute

		//Erstellt IB
		_ib = new IndexBuffer(&_data->_indices[0], _data->_indiceSize, false);

		//Unbindet VAO und VBO
		_vbo1->unbind();
		_vao->unbind();
	}

	void draw() override
	{
		_projection = glm::perspective(glm::radians(_camera->Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f);
		_view = _camera->GetViewMatrix();
		_shader->bind();
		_shader->SetUniformMat4f("model", _model);
		_shader->SetUniformMat4f("projection", _projection);
		_shader->SetUniformMat4f("view", _view);
		_vao->bind();
	}

	void undraw() override
	{
		_shader->unbind();
		_vao->unbind();
	}

	void translate(const glm::vec3& position) override
	{
		_model = glm::translate(_model, position);
	}

	void rotate(const float& angle, const glm::vec3& axis) override
	{
		_model = glm::rotate(_model, glm::radians(angle), axis);
	}

	void scale(const glm::vec3& scalar) override
	{
		_model = glm::scale(_model, scalar);
	}

	unsigned int getNumberOfVertices() override
	{
		return _data->_verticesToRender;
	}

	void updateData(RawData* dataToUse)
	{
		_data = dataToUse;
		initialize();
	}
};