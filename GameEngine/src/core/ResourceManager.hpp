#pragma once

#include <map>
#include "Texture.hpp"
#include "Shader.hpp"
#include "Data.hpp"
#include "MeshCreator.hpp"

class ResourceManager
{
public:
	//------------------------ Texture ------------------------

	//Storing
	static std::map<std::string, Texture*> s_Textures;

	//Loading
	static void LoadTexture(const char* Filepath, const std::string& name)
	{
		s_Textures[name] = new Texture(Filepath);
	}

	//Retrieving
	static Texture* GetTexture(const std::string& name)
	{
		return s_Textures[name];
	}

	//Deleting
	static void DeleteTextures()
	{
		for (auto const& tex : s_Textures)
			delete tex.second;
	}

	//------------------------ Shader ------------------------ 

	//Storing
	static std::map<std::string, Shader*> s_Shaders;

	//Loading
	static void LoadShader(const std::string& vs_Filepath, const std::string& fs_Filepath, const std::string& name)
	{
		s_Shaders[name] = new Shader(vs_Filepath, fs_Filepath);
	}

	//Retrieving
	static Shader* GetShader(const std::string& name)
	{
		return s_Shaders[name];
	}

	//Deleting
	static void DeleteShaders()
	{
		for (auto const& shader : s_Shaders)
			delete shader.second;
	}

	//------------------------ Data ------------------------ 

	//Storing
	static std::map<std::string, Data*> s_Data;

	//Loading
	static void LoadData(const std::string& data_Filepath, const std::string& name)
	{
		s_Data[name] = MeshCreator::loadFromFile(data_Filepath.c_str());
	}

	//Adding
	static void addData(Data* data, const std::string& name)
	{
		s_Data[name] = data;
	}
	
	//Retrieving
	static Data* GetData(const std::string& name)
	{
		return s_Data[name];
	}

	//Deleting
	static void DeleteData()
	{
		for (auto const& data : s_Data)
			delete data.second;
	}
private:
	ResourceManager() {}
};

//Instantiate static variables
std::map<std::string, Texture*> ResourceManager::s_Textures;
std::map<std::string, Shader*>  ResourceManager::s_Shaders;
std::map<std::string, Data*>    ResourceManager::s_Data;