#pragma once

#include "Object.hpp"
#include "ResourceManager.hpp"

class ObjectManager
{
private:
	Object* _object;
	Object* _object2;
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
		delete _object2;
	}

	void init()
	{
		ResourceManager::LoadTexture("res/textures/Brick.jpg", "Brick");
		ResourceManager::LoadShader("res/shader/simulation/object_vs.glsl", "res/shader/simulation/object_fs.glsl", "Object_Shader");
		ResourceManager::LoadData("res/obj/animals/Dog.obj", "Cylinder");
		_object = new Object(ResourceManager::GetTexture("Brick"), ResourceManager::GetShader("Object_Shader"), ResourceManager::GetData("Cylinder"));
		_object2 = new Object(ResourceManager::GetTexture("Brick"), ResourceManager::GetShader("Object_Shader"), ResourceManager::GetData("Cylinder"));
		_object2->translate(glm::vec3(0.0f, 0.0f, 15.0f));
	}
	
	void updateObjects()
	{
		
	}

	void renderObjects()
	{		
		_object->render(glm::vec3(1.0f, 1.0f, 1.0f));
		_object->unrender();
		_object2->render(glm::vec3(1.0f, 0.0f, 0.0f));
		_object2->unrender();
	}
};