#pragma once

#include "Texture.hpp"
#include "Shader.hpp"
#include "VertexBuffer.hpp"
#include "VertexArray.hpp"
#include "IndexBuffer.hpp"

class Object
{
private:
	Texture* _texture = nullptr;
	Shader* _shader = nullptr;
	VertexBuffer* _vbo = nullptr;
	VertexArray* _vao = nullptr;
	IndexBuffer* _ib = nullptr;
	glm::mat4 _model, _view, _projection;
	
	void initRenderData()
	{
		//Create and bind vao
		_vao = new VertexArray();
		_vao->bind();

		float vertices[] =
		{
			-0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 
			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 
			 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 
			 0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 

			-0.5f,  0.5f, 0.5f, 0.0f, 0.0f,
			-0.5f, -0.5f, 0.5f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.5f, 1.0f, 1.0f,
			 0.5f,  0.5f, 0.5f, 1.0f, 0.0f,

			 0.5f,  0.5f, -0.5f, 0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f, 1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f, 1.0f, 0.0f,

			 -0.5f,  0.5f, -0.5f, 0.0f, 0.0f,
			 -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
			 -0.5f, -0.5f,  0.5f, 1.0f, 1.0f,
			 -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,

			 -0.5f, 0.5f,  0.5f, 0.0f, 0.0f,
			 -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
			  0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
			  0.5f, 0.5f,  0.5f, 1.0f, 0.0f,

			 -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
			 -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
			  0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
			  0.5f, -0.5f,  0.5f, 1.0f, 0.0f
		};

		unsigned int indices[] =
		{
			0, 1, 3,
			3, 1, 2,

			4, 5, 7,
			7, 5, 6,

			8, 9, 11,
			11, 9, 10,

			12, 13, 15,
			15, 13, 14,

			16, 17, 19,
			19, 17, 18,

			20, 21, 23,
			23, 21, 22
		};
		
		_vbo = new VertexBuffer(vertices, sizeof(vertices));
		_vao->DefineAttributes(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		_vao->DefineAttributes(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

		_ib = new IndexBuffer(indices, sizeof(indices));
		
		_vbo->unbind();
		_vao->unbind();
	}	
	
public:
	Object(Texture* texture, Shader* shader)
		: _texture(texture), _shader(shader)
	{
		initRenderData();
	}

	~Object()
	{
		delete _vbo;
		delete _vao;
		delete _ib;
	}

	void render(glm::vec3 position, glm::vec3 size, float rotation, glm::vec3 color)
	{
		_shader->bind();

		//Matrices
		_projection = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f);
		_view = camera.GetViewMatrix();
		_model = glm::mat4(1.0f);

		//Model transformations    	
		_model = glm::translate(_model, position);  //First translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)

		_model = glm::translate(_model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); //Move origin of rotation to center of quad
		_model = glm::rotate(_model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)); //Then rotate
		_model = glm::translate(_model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); //Move origin back

		_model = glm::scale(_model, size); //Last scale

		//Set uniforms
		_shader->SetUniformMat4f("projection", _projection);
		_shader->SetUniformMat4f("view", _view);
		_shader->SetUniformMat4f("model", _model);
		_shader->SetUniformVec3("color", color);

		//Set texture
		_texture->bind();

		//Bind vao
		_vao->bind();

		//Render quad
		GLCall(glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr));
	}
};