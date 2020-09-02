#pragma once

#include <vector>

class Data
{
public:
	std::vector<glm::vec3> _vertices;
	std::vector<glm::vec2> _texCoords;
	std::vector<glm::uvec3> _indices;
	std::vector<glm::vec3> _normals;
};