#pragma once

class RawData
{
public:
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
		_verticeData = new float[20]
		{
			-0.5f,  0.5f, 0.0f, 0.0f, 0.0f, // top left
			-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // bottom left
			 0.5f, -0.5f, 0.0f, 1.0f, 1.0f, // bottom right
			 0.5f,  0.5f, 0.0f, 1.0f, 0.0f  // top right 
		};
		_verticeSize = 20 * sizeof(float);

		_indiceData = new unsigned int[6]
		{
			0, 1, 3,   // first triangle
			1, 2, 3    // second triangle
		};
		_indiceSize = 6 * sizeof(unsigned int);
	}
};