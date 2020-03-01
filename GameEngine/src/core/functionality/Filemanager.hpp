#pragma once

#include <spdlog/spdlog.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <fstream>

class Filemanager 
{
public:
	void writeReadableToFile(std::vector<glm::vec3>& vec3_vector, const std::string& filepath)
	{
		std::ofstream myfile(filepath);
		if (myfile.is_open())
		{
			for (glm::vec3 vec3 : vec3_vector)
			{
				myfile << "X: " << vec3.x << " | Y: " << vec3.y << " | Z: " << vec3.z << "\n";
			}

			myfile.close();
			spdlog::info("Erfolgreich in Datei geschrieben! | Path: {}", filepath);
		}
		else spdlog::error("Unable to open file! | Path: {}", filepath);
	}

	void writeReadableToFile(std::vector<glm::uvec3>& uvec3_vector, const std::string& filepath)
	{
		std::ofstream myfile(filepath);
		if (myfile.is_open())
		{
			for (glm::uvec3 uvec3 : uvec3_vector)
			{
				myfile << "P1: " << uvec3.x << " | P2: " << uvec3.y << " | P3: " << uvec3.z << "\n";
			}

			myfile.close();
			spdlog::info("Erfolgreich in Datei geschrieben! | Path: {}", filepath);
		}
		else spdlog::error("Unable to open file! | Path: {}", filepath);
	}

	void writeReadableToFile(std::vector<glm::vec2>& vec2_vector, const std::string& filepath)
	{
		std::ofstream myfile(filepath);
		if (myfile.is_open())
		{
			for (glm::vec2 vec2 : vec2_vector)
			{
				myfile << "X: " << vec2.x << " | Z: " << vec2.y << "\n";
			}

			myfile.close();
			spdlog::info("Erfolgreich in Datei geschrieben! | Path: {}", filepath);
		}
		else spdlog::error("Unable to open file! | Path: {}", filepath);
	}

};