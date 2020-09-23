#pragma once

#include "Random.hpp"

const unsigned int INSTANCES = 250;

class ObjectSpawner
{
private:
	//Actual object instance -> only once
	struct ObjectInstance
	{
		Texture* _texture = nullptr;
		Shader* _shader = nullptr;
		Data* _data = nullptr;
		VertexBuffer* _vbo1 = nullptr, * _vbo2 = nullptr, * _vbo3 = nullptr, * _vbo4 = nullptr;
		VertexArray* _vao = nullptr;
		IndexBuffer* _ib = nullptr;
		glm::mat4 _view, _projection;
		unsigned int _vertices;
		
		ObjectInstance(Texture* texture, Shader* shader, Data* data)
			: _texture(texture), _shader(shader), _data(data)
		{
			
		}

		~ObjectInstance()
		{
			delete _vbo1;
			delete _vbo2;
			delete _vbo3;
			delete _vbo4;

			delete _vao;

			delete _ib;			
		}
	};

	//Single object instance which will get rendered via instancing
	ObjectInstance* _objectInstance = nullptr;
	
	//Create color buffer
	std::vector<glm::vec3> _colorBuffer;

	//Create model buffer
	std::vector<glm::mat4> _modelBuffer;

	//Physics stuff
	PhysicsEngine* _physicsEngine = nullptr;
	std::vector<btRigidBody*> _rigidBodies;
	
	void initData(Texture* texture, Shader* shader, Data* data)
	{	
		//Create instances (differ in color and model matrices)
		for (int i = 0; i < INSTANCES; i++)
		{
			//Color
			_colorBuffer.emplace_back(random::Float(), random::Float(), random::Float());

			//Model matrix
			glm::vec3 pos = glm::vec3(random::Float() * 100.0f, random::Float() * 50.0f, random::Float() * 100.0f);
			_modelBuffer.emplace_back(glm::translate(glm::mat4(1.0f), pos));

			//Add to physics simulation
			_rigidBodies.emplace_back(_physicsEngine->addSphere(pos, 1.0f, 20.0, 0.8f, 1.0f));
		}		

		//Create object
		_objectInstance = new ObjectInstance(texture, shader, data);

		//Create and bind vao
		_objectInstance->_vao = new VertexArray();
		_objectInstance->_vao->bind();
		
		//Create vbo's and configure vao		
		//vbo1 (vertice data)
		_objectInstance->_vbo1 = new VertexBuffer(&_objectInstance->_data->_vertices[0], _objectInstance->_data->_vertices.size() * sizeof(glm::vec3));
		_objectInstance->_vao->DefineAttributes(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

		//vbo2 (texture coordinates)
		_objectInstance->_vbo2 = new VertexBuffer(&_objectInstance->_data->_texCoords[0], _objectInstance->_data->_texCoords.size() * sizeof(glm::vec2));
		_objectInstance->_vao->DefineAttributes(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);

		//vbo3 (colors)
		_objectInstance->_vbo3 = new VertexBuffer(&_colorBuffer[0], _colorBuffer.size() * sizeof(glm::vec3));
		_objectInstance->_vao->DefineAttributes(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
		_objectInstance->_vao->AttributeDivisor(2, 1);

		//vbo4 (model matrix - maximum size for vertex attributes is a vec4 - so we need to send 4 consecutive vec4's to simulate a mat4)
		_objectInstance->_vbo4 = new VertexBuffer(&_modelBuffer[0], INSTANCES * sizeof(glm::mat4), true);
		_objectInstance->_vao->DefineAttributes(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)0);
		_objectInstance->_vao->AttributeDivisor(3, 1);
		_objectInstance->_vao->DefineAttributes(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*) (1 * sizeof(glm::vec4)));
		_objectInstance->_vao->AttributeDivisor(4, 1);
		_objectInstance->_vao->DefineAttributes(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*) (2 * sizeof(glm::vec4)));
		_objectInstance->_vao->AttributeDivisor(5, 1);
		_objectInstance->_vao->DefineAttributes(6, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*) (3 * sizeof(glm::vec4)));		
		_objectInstance->_vao->AttributeDivisor(6, 1);		
		
		//Create ib
		_objectInstance->_ib = new IndexBuffer(&_objectInstance->_data->_indices[0], _objectInstance->_data->_indices.size() * sizeof(glm::uvec3));

		//Calculate vertices to render
		_objectInstance->_vertices = _objectInstance->_data->_indices.size() * 3;
		_verticsToRender = _objectInstance->_vertices * INSTANCES;
		
		//Unbind vao and vbo's
		_objectInstance->_vbo1->unbind();
		_objectInstance->_vbo2->unbind();
		_objectInstance->_vbo3->unbind();
		_objectInstance->_vbo4->unbind();
		_objectInstance->_vao->unbind();
	}
	
public:
	unsigned int _verticsToRender;

	ObjectSpawner(PhysicsEngine* physicsEngine)
		: _physicsEngine(physicsEngine)
	{
		
	}

	~ObjectSpawner()
	{
		delete _objectInstance;
		
		for (btRigidBody* bodies : _rigidBodies)
			delete bodies;
	}
	
	void init(Texture* texture, Shader* shader, Data* data)
	{
		initData(texture, shader, data);
	}

	void render()
	{
		//Do the physics for all instances
		btTransform t;
		for (int i = 0; i < INSTANCES; i++) 
		{
			//Get world transform
			_rigidBodies.at(i)->getMotionState()->getWorldTransform(t);

			//Calculate and set corresponding model matrix
			glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), t.getRotation().getAngle(), glm::vec3(t.getRotation().getAxis().getX(), t.getRotation().getAxis().getY(), t.getRotation().getAxis().getZ()));
			glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(t.getOrigin().getX(), t.getOrigin().getY(), t.getOrigin().getZ()));
			_modelBuffer.at(i) = translation * rotation;
		}

		//Update model matrix in the vertex shader
		_objectInstance->_vbo4->bind();
		_objectInstance->_vbo4->updateData(&_modelBuffer[0], _modelBuffer.size() * sizeof(glm::mat4));
		_objectInstance->_vbo4->unbind();

		//Set matrices
		_objectInstance->_projection = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f);
		_objectInstance->_view = camera.GetViewMatrix();

		//Bind shader
		_objectInstance->_shader->bind();

		//Set uniforms
		_objectInstance->_shader->SetUniformMat4f("projection", _objectInstance->_projection);
		_objectInstance->_shader->SetUniformMat4f("view", _objectInstance->_view);

		//Set texture
		_objectInstance->_texture->bind();

		//Bind vao
		_objectInstance->_vao->bind();

		//Render object instanced
		GLCall(glDrawElementsInstanced(GL_TRIANGLES, (GLsizei)_objectInstance->_vertices, GL_UNSIGNED_INT, nullptr, INSTANCES));
	}
};