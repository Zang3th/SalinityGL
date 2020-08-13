#pragma once

#include <map>
#include "Texture.hpp"

namespace ResourceManager
{
	//Storing
	static std::map<std::string, Texture*> _textures;

	//Loading
	static void LoadTexture(const char* Filepath, const std::string& name)
	{
		_textures[name] = new Texture(Filepath);		
	}

	//Retrieving
	static Texture* GetTexture(const std::string& name)
	{
		return _textures[name];
	}

	//Deleting
	static void DeleteTextures()
	{
		for(auto const& x : _textures)
		{
			delete x.second;
		}
	}
}