#pragma once

#include "Object.hpp"
#include "ResourceManager.hpp"
#include "MeshCreator.hpp"
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

		//Create object spawner and initialize it with allocated resources
		_objectSpawner = new ObjectSpawner(_physicsEngine);
		_objectSpawner->init(ResourceManager::GetTexture("Sphere_texture"), ResourceManager::GetShader("Object_shader"), ResourceManager::GetData("Sphere_data"));
				
		//Plane resources
		unsigned int plane_x = 200;
		unsigned int plane_z = 200;
		unsigned int tile_size = 6;
		ResourceManager::LoadShader("../res/shader/simulation/object_vs.glsl", "../res/shader/simulation/object_fs.glsl", "Object_shader");
		ResourceManager::LoadTexture("../res/textures/Block.jpg", "Block_texture");
		ResourceManager::addData(MeshCreator::createPlane(plane_x / tile_size, plane_z / tile_size, tile_size), "Plane_data");

		//Add plane to renderer and to physics simulation
		{
			//Configure position
			glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f); 

			//Has to be half of the plane size because the physics calculation is based on the center
			glm::vec3 halfsize = glm::vec3((float)plane_x / 2, 0.0f, (float)plane_z / 2); 

			//Add to physics simulation									 
			unsigned int bodyIndex = _physicsEngine->addBox(halfsize, glm::vec3(halfsize.x, 0.1f, halfsize.z), 0.0, 1.0f, 1.0f);

			//Add to renderer
			_objects.emplace_back(
				new Object
					(
						ResourceManager::GetTexture("Block_texture"),
						ResourceManager::GetShader("Object_shader"),
						ResourceManager::GetData("Plane_data"),
						glm::vec3(0.4f, 0.4f, 0.4f),
						position,
						1.0f,
						glm::vec3(0.0f, 0.0f, 0.0f),
						_physicsEngine,
						bodyIndex,
						false //Don't render the plane with physics translation -> causes weird bug
					) 
			);
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