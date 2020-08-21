#pragma once

#include <map>
#include "Texture.hpp"
#include "Shader.hpp"

namespace ResourceManager
{
	//------------------------ Texture ------------------------ 
	
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
		for (auto const& tex : _textures)
			delete tex.second;
	}

	//------------------------ Shader ------------------------ 
	
	//Storing
	static std::map<std::string, Shader*> _shaders;
	
	//Loading
	static void LoadShader(const std::string& vs_Filepath, const std::string& fs_Filepath, const std::string& name)
	{
		_shaders[name] = new Shader(vs_Filepath, fs_Filepath);
	}
	
	//Retrieving
	static Shader* GetShader(const std::string& name)
	{
		return _shaders[name];
	}
		
	//Deleting
	static void DeleteShaders()
	{
		for (auto const& shader : _shaders)
			delete shader.second;
	}
}