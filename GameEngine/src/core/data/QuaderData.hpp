#pragma once

#include "RawData.hpp"

class QuaderData : public RawData
{
public:
	QuaderData()
	{
		init();
	}

	void init()
	{
		float vert[]
		{
			-0.5f,  0.5f, -0.5f, // top left
			-0.5f, -0.5f, -0.5f, // bottom left
			 0.5f, -0.5f, -0.5f, // bottom right
			 0.5f,  0.5f, -0.5f, // top right 

			-0.5f,  0.5f, 0.5f, 
			-0.5f, -0.5f, 0.5f, 
			 0.5f, -0.5f, 0.5f, 
			 0.5f,  0.5f, 0.5f, 

			 0.5f,  0.5f, -0.5f, 
			 0.5f, -0.5f, -0.5f, 
			 0.5f, -0.5f,  0.5f, 
			 0.5f,  0.5f,  0.5f, 

			 -0.5f,  0.5f, -0.5f,
			 -0.5f, -0.5f, -0.5f,
			 -0.5f, -0.5f,  0.5f,
			 -0.5f,  0.5f,  0.5f,

			 -0.5f, 0.5f,  0.5f, 
			 -0.5f, 0.5f, -0.5f, 
			  0.5f, 0.5f, -0.5f, 
			  0.5f, 0.5f,  0.5f, 

			 -0.5f, -0.5f,  0.5f,
			 -0.5f, -0.5f, -0.5f,
			  0.5f, -0.5f, -0.5f,
			  0.5f, -0.5f,  0.5f
		};

		std::vector<float> floats = std::vector<float>(vert, vert + sizeof(vert) / sizeof(vert[0]));

		for(int i = 0; i < floats.size(); i += 3)
		{
			_vertices.emplace_back(glm::vec3(floats.at(i), floats.at(i + 1), floats.at(i + 2)));
		}

		unsigned int ind[] =
		{
			0, 1, 3,
			3, 1, 2,
			4, 5, 7,
			7, 5, 6,
			8, 9, 11,
			11, 9, 10,
			12, 13, 15,
			15, 13, 14,
			16, 17, 19,
			19, 17, 18,
			20, 21, 23,
			23, 21, 22
		};
		
		std::vector<unsigned int> uInts = std::vector<unsigned int>(ind, ind + sizeof(ind) / sizeof(ind[0]));

		for (int i = 0; i < uInts.size(); i += 3)
		{
			_indices.emplace_back(glm::uvec3(uInts.at(i), uInts.at(i + 1), uInts.at(i + 2)));
		}

		setParameters();
	}

	void setParameters()
	{
		_verticeSize = _vertices.size() * sizeof(glm::vec3);
		_indiceSize = _indices.size() * sizeof(glm::uvec3);
		_verticesToRender = (GLsizei)_indices.size() * 3;
	}
};