#pragma once

#include "AssimpLoader.hpp"
#include "Cubemap.hpp"
#include "Shader.hpp"
#include "Standardmodel.hpp"

class CubemapEntity
{
private:
	AssimpLoader* _ai;
	Cubemap* _cubemapTexture;
	unsigned int _textureCount;
	Shader* _shader;
	Standardmodel* _standardmodel;
	friend class EntityManager;
	
public:
	CubemapEntity(unsigned int* nextTextureSlot)
	{
		//Create the data
		_ai = new AssimpLoader("../res/obj/geometry/cube.obj");

		//Create the textures		
		std::vector<const char*> faces
		{
				"../res/textures/cubeMap/xp.jpg",
				"../res/textures/cubeMap/xn.jpg",
				"../res/textures/cubeMap/yp.jpg",
				"../res/textures/cubeMap/yn.jpg",
				"../res/textures/cubeMap/zp.jpg",
				"../res/textures/cubeMap/zn.jpg"
		};
		_textureCount = *nextTextureSlot;
		_cubemapTexture = new Cubemap(faces, _textureCount);
		(*nextTextureSlot)++;
		
		//Create the shader
		_shader = new Shader("../res/shader/zanget3uWorld/cubemap_vs.glsl", "../res/shader/zanget3uWorld/cubemap_fs.glsl");

		//Combine everything to the model
		_standardmodel = new Standardmodel(_ai, _shader, _textureCount, true);
	}

	~CubemapEntity()
	{
		delete _ai;
		delete _cubemapTexture;
		delete _shader;
		delete _standardmodel;
	}
};