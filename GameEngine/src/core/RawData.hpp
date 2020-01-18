#pragma once

#include <vector>

class RawData
{
public:
	int _verticeSize = 0;
	int _indiceSize = 0;
	int _verticesToRender = 0;
	std::vector<unsigned int> _indices;
	std::vector<float> _vertices;	

	void virtual init() = 0;
	void virtual setParameters() = 0;
};