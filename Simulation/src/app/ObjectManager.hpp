#pragma once

#include "Object.hpp"
#include "ResourceManager.hpp"
#include "MeshCreator.hpp"
#include <btBulletDynamicsCommon.h>
#include "PhysicsEngine.hpp"
#include "ObjectSpawner.hpp"

unsigned int VERTICES_TO_RENDER = 0;

class ObjectManager
{
private:
	std::vector<Object*> _objects;
	PhysicsEngine* _physicsEngine = nullptr;
	ObjectSpawner* _objectSpawner = nullptr;
	
public:
	ObjectManager()
	{
		
	}

	~ObjectManager()	
	{		
		ResourceManager::DeleteTextures();
		ResourceManager::DeleteShaders();
		ResourceManager::DeleteData();
		
		for (Object* obj : _objects)
			delete obj;

		delete _physicsEngine;
		delete _objectSpawner;
	}

	void init()
	{
		//Create physics engine
		_physicsEngine = new PhysicsEngine();

		//Allocate resources
		ResourceManager::LoadShader("../res/shader/simulation/object_instanced_vs.glsl", "../res/shader/simulation/object_instanced_fs.glsl", "Object_shader");
		ResourceManager::LoadTexture("../res/textures/models/Moon.jpg", "Sphere_texture");
		ResourceManager::LoadData("../res/obj/geometry/sphere.obj", "Sphere_data");
		_objectSpawner = new ObjectSpawner(_physicsEngine);
		_objectSpawner->init(ResourceManager::GetTexture("Sphere_texture"), ResourceManager::GetShader("Object_shader"), ResourceManager::GetData("Sphere_data"));
				
		//Plane ressources
		unsigned int plane_x = 100;
		unsigned int plane_z = 100;
		ResourceManager::LoadShader("../res/shader/simulation/object_vs.glsl", "../res/shader/simulation/object_fs.glsl", "Object_shader");
		ResourceManager::LoadTexture("../res/textures/Block.jpg", "Block_texture");
		ResourceManager::addData(MeshCreator::createPlane(plane_x, plane_z, 1), "Plane_data");

		//Add plane to renderer and to physics simulation
		{
			//Configure properties
			glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f); 
			glm::vec3 halfsize = glm::vec3((float)plane_x / 2, 0.0f, (float)plane_z / 2); //Has to be half of the plane/box/cube size 
																						  //Physics calculation is based on the center

			//Add to physics simulation									 
			btRigidBody* boxRigidBody = _physicsEngine->addBox(halfsize, glm::vec3(halfsize.x, 0.1f, halfsize.z), 0.0, 1.0f, 1.0f);

			//Add to renderer
			_objects.emplace_back(new Object(
				ResourceManager::GetTexture("Block_texture"),
				ResourceManager::GetShader("Object_shader"),
				ResourceManager::GetData("Plane_data"),
				glm::vec3(1.0f, 1.0f, 1.0f),
				position,
				1.0f,
				Rotation3D(0.0f, 0.0f, 0.0f),
				nullptr) //Don't add to physics calculation -> causes weird translation bug
			);
		}
		
		//Sphere resources
		//ResourceManager::LoadTexture("../res/textures/models/Moon.jpg", "Sphere_texture");
		//ResourceManager::LoadData("../res/obj/geometry/sphere.obj", "Sphere_data");

		//Add sphere to renderer and to physics simulation
		{
			//Configure properties
			//glm::vec3 position = glm::vec3(45.0f, 20.0f, 50.0f);
			//float size = 5.0f;

			////Add to physics simulation
			//btRigidBody* sphereRigidBody = _physicsEngine->addSphere(position, size, 10.0, 0.9f, 1.0f);

			////Add to renderer
			//_objects.emplace_back(new Object(
			//	ResourceManager::GetTexture("Sphere_texture"),
			//	ResourceManager::GetShader("Object_shader"),
			//	ResourceManager::GetData("Sphere_data"),
			//	glm::vec3(1.0f, 1.0f, 1.0f),
			//	position,
			//	size,
			//	Rotation3D(0.0f, 0.0f, 0.0f),
			//	sphereRigidBody)
			//);
		}
		
		//Add sphere to renderer and to physics simulation
		{
			////Configure properties
			//glm::vec3 position = glm::vec3(50.0f, 40.0f, 50.0f);
			//float size = 5.0f;

			////Add to physics simulation
			//btRigidBody* sphereRigidBody = _physicsEngine->addSphere(position, size, 10.0, 0.9f, 1.0f);

			////Add to renderer
			//_objects.emplace_back(new Object(
			//	ResourceManager::GetTexture("Sphere_texture"),
			//	ResourceManager::GetShader("Object_shader"),
			//	ResourceManager::GetData("Sphere_data"),
			//	glm::vec3(1.0f, 1.0f, 1.0f),
			//	position,
			//	size,
			//	Rotation3D(0.0f, 0.0f, 0.0f),
			//	sphereRigidBody)
			//);
		}
		
		//Calculate vertices to render
		for (Object* obj : _objects)
			VERTICES_TO_RENDER += obj->getVertices();

		VERTICES_TO_RENDER += _objectSpawner->_verticsToRender;
	}
	
	void updateObjects()
	{
		_physicsEngine->simulate(deltaTime);
	}

	void renderObjects()
	{
		for (Object* obj : _objects)
			obj->render();

		_objectSpawner->render();
	}
};