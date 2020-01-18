#pragma once

#include "RawData.hpp"

class RawCubeData : public RawData
{
public:
	RawCubeData()
	{
		init();
	}

	void init() override
	{
		float vert[]
		{
			-0.5f,  0.5f, -0.5f, 0.0f, 0.0f, // top left
			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // bottom left
			 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, // bottom right
			 0.5f,  0.5f, -0.5f, 1.0f, 0.0f, // top right 

			-0.5f,  0.5f, 0.5f, 0.0f, 0.0f,
			-0.5f, -0.5f, 0.5f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.5f, 1.0f, 1.0f,
			 0.5f,  0.5f, 0.5f, 1.0f, 0.0f,

			 0.5f,  0.5f, -0.5f, 0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f, 1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f, 1.0f, 0.0f,

			 -0.5f,  0.5f, -0.5f, 0.0f, 0.0f,
			 -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
			 -0.5f, -0.5f,  0.5f, 1.0f, 1.0f,
			 -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,

			 -0.5f, 0.5f,  0.5f, 0.0f, 0.0f,
			 -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
			  0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
			  0.5f, 0.5f,  0.5f, 1.0f, 0.0f,

			 -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
			 -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
			  0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
			  0.5f, -0.5f,  0.5f, 1.0f, 0.0f
		};
		_vertices = std::vector<float>(vert, vert + sizeof(vert) / sizeof(vert[0]));		

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
		_indices = std::vector<unsigned int>(ind, ind + sizeof(ind) / sizeof(ind[0]));
		
		setParameters();
	}

	void setParameters() override
	{
		_verticeSize = _vertices.size() * sizeof(float);
		_indiceSize = _indices.size() * sizeof(unsigned int);
		_verticesToRender = _indices.size() * sizeof(unsigned int);
	}
};