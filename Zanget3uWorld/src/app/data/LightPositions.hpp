#pragma once

#include <glm/vec3.hpp>

const int numberOfPointlights = 9;

//Lightbulbpositions for the shaders (Lanternpositions in y are 23.0f lower on the same x and z coordinates)
glm::vec3 _lightPositions[numberOfPointlights] =
{
	glm::vec3(374.0f, 20.0f, 76.0f),
	glm::vec3(209.0f, 31.0f, 86.0f),
	glm::vec3(61.0f, 23.5f, 58.0f),
	glm::vec3(141.0f, 19.0f, 237.0f),
	glm::vec3(75.0f, 33.0f, 373.0f),
	glm::vec3(210.0f, 26.0f, 462.0f),
	glm::vec3(345.0f, 23.0f, 430.0f),
	glm::vec3(454.0f, 33.5f, 335.0f),
	glm::vec3(357.0f, 23.0f, 226.0f)
};

glm::vec3 _lightColor = glm::vec3(1.0f, 1.0f, 1.0f);