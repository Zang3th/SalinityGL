#pragma once

#include "Texture.hpp"
#include "Shader.hpp"
#include "VertexBuffer.hpp"
#include "VertexArray.hpp"

class Object
{
private:
	Texture* _texture = nullptr;
	Shader* _shader = nullptr;
	VertexBuffer* _vbo = nullptr;
	VertexArray* _vao = nullptr;
	glm::mat4 _projection;
	
	void initRenderData()
	{
		//Create and bind vao
		_vao = new VertexArray();
		_vao->bind();

		float vertices[] =
		{
			//Position  //Texture
			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,

			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f
		};

		_vbo = new VertexBuffer(vertices, sizeof(vertices));
		_vao->DefineAttributes(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

		_vbo->unbind();
		_vao->unbind();
	}
	
	void matrixSetUP()
	{
		//Projection-Matrix
		_projection = glm::ortho(0.0f, (float)WIDTH, (float)HEIGHT, 0.0f, -1.0f, 1.0f);
	}
	
public:
	Object(Texture* texture, Shader* shader)
		: _texture(texture), _shader(shader)
	{
		initRenderData();
		matrixSetUP();
	}

	~Object()
	{
		delete _vbo;
		delete _vao;
	}

	void render(glm::vec2 position, glm::vec2 size, float rotation, glm::vec3 color)
	{
		_shader->bind();

		//Model-Matrix
		glm::mat4 model = glm::mat4(1.0f);

		//Model transformations    	
		model = glm::translate(model, glm::vec3(position, 0.0f));  //First translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)

		model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); //Move origin of rotation to center of quad
		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)); //Then rotate
		model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); //Move origin back

		model = glm::scale(model, glm::vec3(size, 1.0f)); //Last scale

		//Set uniforms
		_shader->SetUniformMat4f("projection", _projection);
		_shader->SetUniformMat4f("model", model);
		_shader->SetUniformVec3("color", color);

		//Set texture
		_texture->bind();

		//Bind vao
		_vao->bind();

		//Render quad
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));
	}
};