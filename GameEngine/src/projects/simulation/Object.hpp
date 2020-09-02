#pragma once

#include "Texture.hpp"
#include "Shader.hpp"
#include "Data.hpp"
#include "VertexBuffer.hpp"
#include "VertexArray.hpp"
#include "IndexBuffer.hpp"

class Object
{
private:
	Texture* _texture = nullptr;
	Shader* _shader = nullptr;
	Data* _data = nullptr;
	VertexBuffer* _vbo = nullptr;
	VertexArray* _vao = nullptr;
	IndexBuffer* _ib = nullptr;
	glm::mat4 _model, _view, _projection;
	
	void initRenderData()
	{
		//Create and bind vao
		_vao = new VertexArray();
		_vao->bind();

		//Create vbo and configure vao
		_vbo = new VertexBuffer(&_data->_vertices[0], _data->_vertices.size() * sizeof(glm::vec3));
		_vao->DefineAttributes(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

		//Create ib
		_ib = new IndexBuffer(&_data->_indices[0], _data->_indices.size() * sizeof(glm::uvec3));

		//Unbind vao and vbo
		_vbo->unbind();
		_vao->unbind();
	}	
	
public:
	Object(Texture* texture, Shader* shader, Data* data)
		: _texture(texture), _shader(shader), _data(data)
	{
		initRenderData();
		_model = glm::mat4(1.0f);
	}

	~Object()
	{
		delete _vbo;
		delete _vao;
		delete _ib;
	}

	void render(const glm::vec3& color)
	{
		//Matrices
		_projection = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f);
		_view = camera.GetViewMatrix();
		//_model = glm::mat4(1.0f);
		
		_shader->bind();		

		//Set uniforms
		_shader->SetUniformMat4f("model", _model);
		_shader->SetUniformMat4f("projection", _projection);
		_shader->SetUniformMat4f("view", _view);		
		_shader->SetUniformVec3("color", color);

		//Set texture
		_texture->bind();

		//Bind vao
		_vao->bind();

		//Render object
		GLCall(glDrawElements(GL_TRIANGLES, (GLsizei)_data->_indices.size() * 3, GL_UNSIGNED_INT, nullptr));
	}

	void unrender()
	{
		_shader->unbind();
		_vao->unbind();
	}
	
	void translate(const glm::vec3& position)
	{
		_model = glm::translate(_model, position);
	}
};