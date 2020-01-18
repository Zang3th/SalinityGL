#pragma once

class RawData
{
public:	
	const unsigned int _pos_coords = 3;
	const unsigned int _tex_coords = 2;
	const unsigned int _verticesPerSide = 4;
	const unsigned int _totalVerticesPerSide = 6;
	const unsigned int _sides = 6;
	const unsigned int _triangles = 2 * _sides;
	const unsigned int _numberOfVertices = (_pos_coords + _tex_coords) * _verticesPerSide * _sides;
	const unsigned int _numberOfIndices = _triangles * 3;
	float *_verticeData = nullptr;
	unsigned int* _indiceData = nullptr;
	int _verticeSize = 0;
	int _indiceSize = 0;

	RawData()
	{
		
	}

	~RawData()
	{
		delete _verticeData;
		delete _indiceData;
	}

	void initializeCubeData()
	{
		_verticeData = new float[_numberOfVertices]
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
		_verticeSize = _numberOfVertices * sizeof(float);

		_indiceData = new unsigned int[_numberOfIndices]
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
		_indiceSize = _numberOfIndices * sizeof(unsigned int);
	}
};