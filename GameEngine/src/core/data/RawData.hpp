#pragma once

#include <vector>

class RawData
{
public:
	int _verticeSize = 0;
	int _indiceSize = 0;
	int _texCoordSize = 0;
	int _blendmapCoordsSize = 0;
	int _normalSize = 0;
	int _verticesToRender = 0;
	int _isPickedSize = 0;

	std::vector<glm::vec3> _vertices;
	std::vector<glm::uvec3> _indices;
	std::vector<glm::vec2> _texCoords;
	std::vector<glm::vec2> _blendmapCoords;
	std::vector<glm::vec3> _normals;
	std::vector<glm::vec3> _isPicked;
};