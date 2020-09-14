#pragma once

#include "Object.hpp"
#include "ResourceManager.hpp"
#include "MeshCreator.hpp"
#include <btBulletDynamicsCommon.h>
#include "PhysicsEngine.hpp"

unsigned int VERTICES_TO_RENDER = 0;

class ObjectManager
{
private:
	std::vector<Object*> _objects;
	PhysicsEngine* _physicsEngine = nullptr;

public:
	ObjectManager()
	{
		
	}

	~ObjectManager()	
	{		
		ResourceManager::DeleteTextures();
		ResourceManager::DeleteShaders();
		ResourceManager::DeleteData();

		delete _physicsEngine;

		for (Object* obj : _objects)
			delete obj;
	}

	void init()
	{
		//Create physics engine
		_physicsEngine = new PhysicsEngine();

		//Object resources
		//ResourceManager::LoadTexture("res/textures/models/Duck.jpg", "Duck_texture");
		ResourceManager::LoadShader("../res/shader/simulation/object_vs.glsl", "../res/shader/simulation/object_fs.glsl", "Object_shader");
		//ResourceManager::LoadData("res/obj/animals/Duck.obj", "Duck_data");

		//Add objects to renderer
		{
			/*_objects.emplace_back(new Object(
				ResourceManager::GetTexture("Duck_texture"),
				ResourceManager::GetShader("Object_shader"),
				ResourceManager::GetData("Duck_data"),
				glm::vec3(1.0f, 1.0f, 1.0f),
				glm::vec3(0.0f, 0.0f, 0.0f), 
				0.2f,
				Rotation3D(0.0f, 45.0f, 0.0f))
			);
			
			_objects.emplace_back(new Object(
				ResourceManager::GetTexture("Duck_texture"),
				ResourceManager::GetShader("Object_shader"),
				ResourceManager::GetData("Duck_data"),
				glm::vec3(1.0f, 1.0f, 1.0f),
				glm::vec3(0.0f, 0.0f, 100.0f), 
				0.2f,
				Rotation3D(0.0f, 135.0f, 0.0f))
			);

			_objects.emplace_back(new Object(
				ResourceManager::GetTexture("Duck_texture"),
				ResourceManager::GetShader("Object_shader"),
				ResourceManager::GetData("Duck_data"),
				glm::vec3(1.0f, 1.0f, 1.0f),
				glm::vec3(100.0f, 0.0f, 100.0f), 
				0.2f, 
				Rotation3D(0.0f, -135.0f, 0.0f))
			);

			_objects.emplace_back(new Object(
				ResourceManager::GetTexture("Duck_texture"),
				ResourceManager::GetShader("Object_shader"),
				ResourceManager::GetData("Duck_data"),
				glm::vec3(1.0f, 1.0f, 1.0f),
				glm::vec3(100.0f, 0.0f, 0.0f), 
				0.2f, 
				Rotation3D(0.0f, -45.0f, 0.0f))
			);*/
		}
		
		//Plane ressources
		ResourceManager::LoadTexture("../res/textures/Block.jpg", "Block_texture");
		ResourceManager::addData(MeshCreator::createPlane(25, 25, 4), "Plane_data");

		//Add plane to renderer and to physics simulation
		{
			//Configure properties
			glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
			float size = 1.0f;

			//Add to phyisics simulation
			btRigidBody* boxRigidBody = _physicsEngine->addBox(position, glm::vec3(100.0f, 0.5f, 100.0f), 0.0, 1.0f, 1.0f);

			//Add to renderer
			_objects.emplace_back(new Object(
				ResourceManager::GetTexture("Block_texture"),
				ResourceManager::GetShader("Object_shader"),
				ResourceManager::GetData("Plane_data"),
				glm::vec3(1.0f, 1.0f, 1.0f),
				position,
				size,
				Rotation3D(0.0f, 0.0f, 0.0f),
				boxRigidBody)
			);
		}
		
		//Sphere ressources
		ResourceManager::LoadTexture("../res/textures/models/Moon.jpg", "Sphere_texture");
		ResourceManager::LoadData("../res/obj/geometry/sphere.obj", "Sphere_data");

		//Add sphere to renderer and to physics simulation
		{
			//Configure properties
			glm::vec3 position = glm::vec3(50.0f, 20.0f, 50.0f);
			float size = 5.0f;

			//Add to phyisics simulation
			btRigidBody* sphereRigidBody = _physicsEngine->addSphere(position, size, 10.0, 0.9f, 1.0f);

			//Add to renderer
			_objects.emplace_back(new Object(
				ResourceManager::GetTexture("Sphere_texture"),
				ResourceManager::GetShader("Object_shader"),
				ResourceManager::GetData("Sphere_data"),
				glm::vec3(1.0f, 1.0f, 1.0f),
				position,
				size,
				Rotation3D(0.0f, 0.0f, 0.0f),
				sphereRigidBody)
			);
		}

		//Calculate vertices to render
		for (Object* obj : _objects)
			VERTICES_TO_RENDER += obj->getVertices();
	}
	
	void updateObjects()
	{
		_physicsEngine->simulate(deltaTime);
	}

	void renderObjects()
	{
		for (Object* obj : _objects)
			obj->render();
	}
};