#pragma once

#include "AssimpLoader.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "Standardmodel.hpp"
#include "Basemodel.hpp"

class ObjmodelEntity
{
private:
	AssimpLoader* _ai;
	Texture* _texture;
	unsigned int _textureCount;
	Shader* _shader;
	Standardmodel* _standardmodel;
	
public:	
	ObjmodelEntity(const char* objFile, const char* texture, const char* vsShader, const char* fsShader, unsigned int* nextTextureSlot)
	{
		//Create the data
		_ai = new AssimpLoader(objFile);
		
		//Create the texture
		_textureCount = *nextTextureSlot;
		_texture = new Texture(texture, _textureCount);
		(*nextTextureSlot)++;
		
		//Create the shader
		_shader = new Shader(vsShader, fsShader);
		
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

	Standardmodel* getModel()
	{
		return _standardmodel;
	}
};