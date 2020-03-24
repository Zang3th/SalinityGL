#pragma once

#include "AssimpLoader.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "Standardmodel.hpp"

class ObjmodelEntity
{
private:
	AssimpLoader* _ai;
	Texture* _texture;
	unsigned int _textureCount;
	Shader* _shader;
	Standardmodel* _standardmodel;
	friend class EntityManager;
	
public:	
	ObjmodelEntity(const char* objFile, const char* texture, unsigned int* nextTextureSlot)
	{
		//Create the data
		_ai = new AssimpLoader(objFile);
		
		//Create the texture
		_textureCount = *nextTextureSlot;
		_texture = new Texture(texture, _textureCount);
		(*nextTextureSlot)++;
		
		//Create the shader
		_shader = new Shader("res/shader/standard_vs.glsl", "res/shader/standard_fs.glsl");
		
		//Combine everything to the model
		_standardmodel = new Standardmodel(_ai, _shader, _textureCount, false);
	}

	~ObjmodelEntity()
	{
		delete _ai;
		delete _texture;
		delete _shader;
		delete _standardmodel;
	}
	
	void translate(const glm::vec3& tVec3) const
	{
		_standardmodel->translate(tVec3);
	}

	void rotate(const float& angle, const glm::vec3& axis) const
	{
		_standardmodel->rotate(angle, axis);
	}

	void scale(const glm::vec3& scalar) const
	{
		_standardmodel->scale(scalar);
	}
};