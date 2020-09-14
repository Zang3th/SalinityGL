#pragma once

#include "Player.hpp"

class PlayerEntity
{
private:
	AssimpLoader* _playerData;
	Texture* _playerTex;
	unsigned int _textureCount;
	Shader* _shader;
	Standardmodel* _playerModel;
	friend class EntityManager;
	
public:
	PlayerEntity(DisplayManager* DM, AudioManager* AM, const glm::vec3& spawnPos, GroundData* GD, unsigned int* nextTextureSlot)
	{
		//Create the data
		_playerData = new AssimpLoader("../res/obj/humans/Chibi.obj");
		
		//Create the texture
		_textureCount = *nextTextureSlot;
		_playerTex = new Texture("../res/textures/models/Chibi.jpg", _textureCount);
		(*nextTextureSlot)++;
		
		//Create the shader
		_shader = new Shader("../res/shader/zanget3uWorld/standard_vs.glsl", "../res/shader/zanget3uWorld/standard_fs.glsl");

		//Combine everything to the model
		_playerModel = new Standardmodel(_playerData, _shader, _textureCount);

		DM->_player = new Player(_playerModel, spawnPos, GD, AM);
	}

	~PlayerEntity()
	{
		delete _playerData;
		delete _playerTex;
		delete _shader;
		delete _playerModel;
	}
};