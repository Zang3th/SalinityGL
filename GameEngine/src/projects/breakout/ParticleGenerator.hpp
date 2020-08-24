#pragma once

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include "Shader.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"

class ParticleGenerator
{
private:
	struct Particle
	{
		glm::vec2 _position, _velocity, _size;
		glm::vec4 _color;
		float _life;

		Particle(glm::vec2 position, glm::vec2 velocity, glm::vec2 size, glm::vec4 color, float life)
			: _position(position), _velocity(velocity), _size(size), _color(color), _life(life)
		{

		}
	};

	void initRenderData()
	{
		//Erstellt und bindet VAO
		_vao = new VertexArray();
		_vao->bind();

		float vertices[] =
		{
			0.0f, 1.0f,
			1.0f, 0.0f,
			0.0f, 0.0f,

			0.0f, 1.0f,
			1.0f, 1.0f,
			1.0f, 0.0f
		};

		_vbo = new VertexBuffer(vertices, sizeof(vertices));
		_vao->DefineAttributes(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

		_vbo->unbind();
		_vao->unbind();
	}	

	void createParticles()
	{
		_particles.emplace_back(glm::vec2(900.0f, 600.0f), glm::vec2(0.0f, 0.0f), glm::vec2(50.0f, 50.0f), glm::vec4(0.9f, 0.9f, 0.0f, 0.0f), 0.0f);
	}
	
	VertexArray* _vao = nullptr;
	VertexBuffer* _vbo = nullptr;
	Shader* _shader = nullptr;
	glm::mat4 _projection;
	std::vector<Particle> _particles;
	
public:	
	ParticleGenerator(Shader* shader, glm::mat4 projectionMatrix)
		: _shader(shader), _projection(projectionMatrix)
	{
		this->initRenderData();
		this->createParticles();
	}

	~ParticleGenerator()
	{
		delete _vao;
		delete _vbo;
	}

	void updateParticles()
	{
		
	}

	void renderParticles()
	{
		for (Particle p : _particles)
		{
			_shader->bind();

			//Model transformations
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(p._position, 0.0f)); //translate
			model = glm::scale(model, glm::vec3(p._size, 1.0f)); //scale
			
			//Set Uniforms
			_shader->SetUniformMat4f("projection", _projection);
			_shader->SetUniformMat4f("model", model);
			_shader->SetUniformVec3("particleColor", p._color);

			//Bind vao
			_vao->bind();

			//Render quad
			GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));
		}
	}
};