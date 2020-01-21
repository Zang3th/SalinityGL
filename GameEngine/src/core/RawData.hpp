#pragma once

#include <vector>

class RawData
{
public:
	int _verticeSize = 0;
	int _indiceSize = 0;
	int _texCoordSize = 0;
	int _verticesToRender = 0;
	int _terrainColorSize = 0;
	std::vector<glm::uvec3> _indices;
	std::vector<glm::vec3> _vertices;	
	std::vector<glm::vec2> _texCoords;	
	std::vector<glm::vec3> _terrainColors;

	void virtual init() = 0;
	void virtual setParameters() = 0;
};