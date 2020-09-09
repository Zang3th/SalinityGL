#pragma once

#include "Object.hpp"
#include "ResourceManager.hpp"
#include "MeshCreator.hpp"

class ObjectManager
{
private:
	Object *_object = nullptr, *_plane;
	
public:
	ObjectManager()
	{
		
	}

	~ObjectManager()	
	{		
		ResourceManager::DeleteTextures();
		ResourceManager::DeleteShaders();
		ResourceManager::DeleteData();
		delete _object;
		delete _plane;
	}

	void init()
	{
		//Object
		ResourceManager::LoadTexture("res/textures/models/Duck.jpg", "Duck_texture");
		ResourceManager::LoadShader("res/shader/simulation/object_vs.glsl", "res/shader/simulation/object_fs.glsl", "Object_shader");
		ResourceManager::LoadData("res/obj/animals/Duck.obj", "Duck_data");
		_object = new Object(ResourceManager::GetTexture("Duck_texture"), ResourceManager::GetShader("Object_shader"), ResourceManager::GetData("Duck_data"));

		//Plane
		ResourceManager::LoadTexture("res/textures/Block.jpg", "Block_texture");
		ResourceManager::addData(MeshCreator::createPlane(25, 25, 4), "Plane_data");
		_plane = new Object(ResourceManager::GetTexture("Block_texture"), ResourceManager::GetShader("Object_shader"), ResourceManager::GetData("Plane_data"));
	}
	
	void updateObjects()
	{
		
	}

	void renderObjects()
	{
		//Object
		_object->render(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f),0.2f, Rotation3D(0.0f, 45.0f, 0.0f));
		_object->render(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 100.0f), 0.2f, Rotation3D(0.0f, 135.0f, 0.0f));
		_object->render(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(100.0f, 0.0f, 100.0f), 0.2f, Rotation3D(0.0f, -135.0f, 0.0f));
		_object->render(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(100.0f, 0.0f, 0.0f), 0.2f, Rotation3D(0.0f, -45.0f, 0.0f));

		//Plane
		_plane->render(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f, Rotation3D(0.0f, 0.0f, 0.0f));
	}
};