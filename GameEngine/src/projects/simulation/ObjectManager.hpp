#pragma once

#include "Object.hpp"
#include "ResourceManager.hpp"

class ObjectManager
{
private:
	Object* _object;
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
	}

	void init()
	{
		ResourceManager::LoadTexture("res/textures/Stone_2.jpg", "Brick");
		ResourceManager::LoadShader("res/shader/simulation/object_vs.glsl", "res/shader/simulation/object_fs.glsl", "Object_Shader");
		ResourceManager::LoadData("res/obj/humans/Chibi.obj", "Chibi");
		_object = new Object(ResourceManager::GetTexture("Brick"), ResourceManager::GetShader("Object_Shader"), ResourceManager::GetData("Chibi"));
	}
	
	void updateObjects()
	{
		
	}

	void renderObjects()
	{
		_object->render(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f),3.0f, 45.0f);
		_object->render(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 100.0f), 3.0f, 135.0f);
		_object->render(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(100.0f, 0.0f, 100.0f), 3.0f, -135.0f);
		_object->render(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(100.0f, 0.0f, 0.0f), 3.0f, -45.0f);
	}
};