#pragma once

#include <glm/vec3.hpp>

const int numberOfPointlights = 9;

//Lightbulbs
glm::vec3 _lightPositions[numberOfPointlights] =
{
	glm::vec3(373.9f, 14.0f, 76.0f),
	glm::vec3(248.9f, 17.5f, 112.0f),
	glm::vec3(100.9f, 17.5f, 58.0f),
	glm::vec3(143.9f, 14.0f, 194.0f),
	glm::vec3(74.9f, 22.0f, 293.0f),
	glm::vec3(139.9f, 23.0f, 427.0f),
	glm::vec3(344.9f, 20.0f, 430.0f),
	glm::vec3(453.9f, 20.5f, 335.0f),
	glm::vec3(326.9f, 21.0f, 238.0f)
};
glm::vec3 _lightScale = glm::vec3(0.5f, 0.5f, 0.5f);
glm::vec3 _lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

//Lanterns
glm::vec3 _lanternPositions[numberOfPointlights] =
{
	glm::vec3(350.0f, -9.0f, 75.0f),
	glm::vec3(225.0f, -5.5f, 111.0f),
	glm::vec3(77.0f, -5.5f, 57.0f),
	glm::vec3(120.0f, -9.0f, 193.0f),
	glm::vec3(51.0f, -1.0f, 292.0f),
	glm::vec3(116.0f, 0.0f, 426.0f),
	glm::vec3(321.0f, -3.0f, 429.0f),
	glm::vec3(430.0f, -2.5f, 334.0f),
	glm::vec3(303.0f, -2.0f, 237.0f)
};
glm::vec3 _lanternScale = glm::vec3(3.0f, 3.0f, 3.0f);
