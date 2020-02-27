#pragma once

#include <glm/vec3.hpp>

const int numberOfPointlights = 9;
glm::vec3 _lightPositions[numberOfPointlights] =
{
	glm::vec3(375.0f, 10.0f, 75.0f),
	glm::vec3(252.0f, 10.0f, 113.0f),
	glm::vec3(100.0f, 10.0f, 58.0f),
	glm::vec3(143.0f, 10.0f, 194.0f),
	glm::vec3(74.0f, 20.0f, 293.0f),
	glm::vec3(139.0f, 20.0f, 427.0f),
	glm::vec3(344.0f, 20.0f, 430.0f),
	glm::vec3(455.0f, 20.0f, 335.0f),
	glm::vec3(326.0f, 20.0f, 238.0f)
};
glm::vec3 _lightScale = glm::vec3(0.5f, 0.5f, 0.5f);
glm::vec3 _lightColor = glm::vec3(1.0f, 1.0f, 1.0f);