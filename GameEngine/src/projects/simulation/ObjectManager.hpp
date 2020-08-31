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
		delete _object;
	}

	void init()
	{
		ResourceManager::LoadTexture("res/textures/Brick.jpg", "Brick");
		ResourceManager::LoadShader("res/shader/simulation/object_vs.glsl", "res/shader/simulation/object_fs.glsl", "Object_Shader");
		_object = new Object(ResourceManager::GetTexture("Brick"), ResourceManager::GetShader("Object_Shader"));
	}
	
	void updateObjects()
	{
		
	}

	void renderObjects()
	{
		_object->render(glm::vec2(500.0f, 500.0f), glm::vec2(10.0f, 10.0f), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
	}
};