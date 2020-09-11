#pragma once

#include "Object.hpp"
#include "ResourceManager.hpp"
#include "MeshCreator.hpp"

unsigned int VERTICES_TO_RENDER = 0;

class ObjectManager
{
private:
	std::vector<Object*> _objects;
	
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
	}

	void init()
	{
		//Object ressources
		ResourceManager::LoadTexture("res/textures/models/Duck.jpg", "Duck_texture");
		ResourceManager::LoadShader("res/shader/simulation/object_vs.glsl", "res/shader/simulation/object_fs.glsl", "Object_shader");
		ResourceManager::LoadData("res/obj/animals/Duck.obj", "Duck_data");

		//Add objects to renderer
		{
			_objects.emplace_back(new Object(
				ResourceManager::GetTexture("Duck_texture"),
				ResourceManager::GetShader("Object_shader"),
				ResourceManager::GetData("Duck_data"),
				glm::vec3(1.0f, 1.0f, 1.0f),
				glm::vec3(0.0f, 0.0f, 0.0f), 0.2f,
				Rotation3D(0.0f, 45.0f, 0.0f))
			);
			
			_objects.emplace_back(new Object(
				ResourceManager::GetTexture("Duck_texture"),
				ResourceManager::GetShader("Object_shader"),
				ResourceManager::GetData("Duck_data"),
				glm::vec3(1.0f, 1.0f, 1.0f),
				glm::vec3(0.0f, 0.0f, 100.0f), 0.2f,
				Rotation3D(0.0f, 135.0f, 0.0f))
			);

			_objects.emplace_back(new Object(
				ResourceManager::GetTexture("Duck_texture"),
				ResourceManager::GetShader("Object_shader"),
				ResourceManager::GetData("Duck_data"),
				glm::vec3(1.0f, 1.0f, 1.0f),
				glm::vec3(100.0f, 0.0f, 100.0f), 0.2f, 
				Rotation3D(0.0f, -135.0f, 0.0f))
			);

			_objects.emplace_back(new Object(
				ResourceManager::GetTexture("Duck_texture"),
				ResourceManager::GetShader("Object_shader"),
				ResourceManager::GetData("Duck_data"),
				glm::vec3(1.0f, 1.0f, 1.0f),
				glm::vec3(100.0f, 0.0f, 0.0f), 0.2f, 
				Rotation3D(0.0f, -45.0f, 0.0f))
			);
		}
		
		//Plane ressources
		ResourceManager::LoadTexture("res/textures/Block.jpg", "Block_texture");
		ResourceManager::addData(MeshCreator::createPlane(25, 25, 4), "Plane_data");

		//Add plane to renderer
		{
			_objects.emplace_back(new Object(
				ResourceManager::GetTexture("Block_texture"),
				ResourceManager::GetShader("Object_shader"),
				ResourceManager::GetData("Plane_data"),
				glm::vec3(1.0f, 1.0f, 1.0f),
				glm::vec3(0.0f, 0.0f, 0.0f), 1.0f,
				Rotation3D(0.0f, 0.0f, 0.0f))
			);
		}
		
		//Sphere ressources
		ResourceManager::LoadTexture("res/textures/models/Moon.jpg", "Sphere_texture");
		ResourceManager::LoadData("res/obj/geometry/sphere.obj", "Sphere_data");

		//Add sphere to renderer
		{
			_objects.emplace_back(new Object(
				ResourceManager::GetTexture("Sphere_texture"),
				ResourceManager::GetShader("Object_shader"),
				ResourceManager::GetData("Sphere_data"),
				glm::vec3(1.0f, 1.0f, 1.0f),
				glm::vec3(50.0f, 30.0f, 50.0f), 10.0f,
				Rotation3D(0.0f, 0.0f, 0.0f))
			);
		}

		//Calculate vertices to render
		for (Object* obj : _objects)
			VERTICES_TO_RENDER += obj->getVertices();
	}
	
	void updateObjects()
	{
		
	}

	void renderObjects()
	{
		for (Object* obj : _objects)
			obj->render();
	}
};