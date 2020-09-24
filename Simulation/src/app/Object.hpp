#pragma once

#include "Texture.hpp"
#include "Shader.hpp"
#include "Data.hpp"
#include "VertexBuffer.hpp"
#include "VertexArray.hpp"
#include "IndexBuffer.hpp"
#include "PhysicsEngine.hpp"

class Object
{
private:
	Texture *_texture = nullptr;
	Shader *_shader = nullptr;
	Data *_data = nullptr;
	VertexBuffer *_vbo = nullptr, *_vbo2 = nullptr;
	VertexArray *_vao = nullptr;
	IndexBuffer *_ib = nullptr;
	glm::mat4 _model, _view, _projection;
	glm::vec3 _color, _initPos, _initRotation;
	unsigned int _vertices, _bodyIndex;
	float _size;
	PhysicsEngine* _physicsEngine = nullptr;
	bool _translatePhysics;

	void initRenderData()
	{
		//Create and bind vao
		_vao = new VertexArray();
		_vao->bind();
		
		//Create vbo and configure vao
		_vbo = new VertexBuffer(&_data->_vertices[0], _data->_vertices.size() * sizeof(glm::vec3));
		_vao->DefineAttributes(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
		_vbo2 = new VertexBuffer(&_data->_texCoords[0], _data->_texCoords.size() * sizeof(glm::vec2));
		_vao->DefineAttributes(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
		
		//Create ib
		_ib = new IndexBuffer(&_data->_indices[0], _data->_indices.size() * sizeof(glm::uvec3));

		//Vertices to render
		_vertices = _data->_indices.size() * 3;
		
		//Unbind vao and vbo
		_vbo->unbind();
		_vbo2->unbind();
		_vao->unbind();
	}	
	
public:
	Object(Texture* texture, Shader* shader, Data* data, const glm::vec3& color, const glm::vec3& translation, const float& scalar, const glm::vec3& rotation, PhysicsEngine* physicsEngine, unsigned int bodyIndex, bool translatePhysics)
		: _texture(texture), _shader(shader), _data(data), _color(color), _initPos(translation), _size(scalar), _initRotation(rotation), _bodyIndex(bodyIndex), _physicsEngine(physicsEngine), _translatePhysics(translatePhysics)
	{
		initRenderData();		
		
		//Transformations
		_model = glm::mat4(1.0f);
		_model = glm::translate(_model, _initPos);
		//_model = glm::rotate(_model, glm::radians(_initRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		//_model = glm::rotate(_model, glm::radians(_initRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		//_model = glm::rotate(_model, glm::radians(_initRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		_model = glm::scale(_model, glm::vec3(_size));
	}

	~Object()
	{
		delete _vbo;
		delete _vbo2;
		delete _vao;
		delete _ib;
	}

	void render()
	{
		if (_translatePhysics)
		{
			_model = _physicsEngine->getWorldTransform(_bodyIndex);
			_model = glm::scale(_model, glm::vec3(_size));
		}

		//Matrices
		_projection = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f);
		_view = camera.GetViewMatrix();

		_shader->bind();

		//Set uniforms
		_shader->SetUniformMat4f("model", _model);
		_shader->SetUniformMat4f("projection", _projection);
		_shader->SetUniformMat4f("view", _view);
		_shader->SetUniformVec3("color", _color);

		//Set texture
		_texture->bind();

		//Bind vao
		_vao->bind();

		//Render object
		GLCall(glDrawElements(GL_TRIANGLES, (GLsizei)_vertices, GL_UNSIGNED_INT, nullptr));
	}

	unsigned int getVertices() const
	{
		return _vertices;
	}
};