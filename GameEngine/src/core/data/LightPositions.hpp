#pragma once

#include <glm/vec3.hpp>

const int numberOfPointlights = 9;

//Lightbulbs
glm::vec3 _lightPositions[numberOfPointlights] =
{
	glm::vec3(373.9f, 20.0f, 76.0f),
	glm::vec3(208.9f, 31.0f, 86.0f),
	glm::vec3(60.9f, 23.5f, 58.0f),
	glm::vec3(143.9f, 18.0f, 237.0f),
	glm::vec3(74.9f, 32.0f, 373.0f),
	glm::vec3(209.9f, 26.0f, 462.0f),
	glm::vec3(344.9f, 23.0f, 430.0f),
	glm::vec3(453.9f, 31.5f, 335.0f),
	glm::vec3(356.9f, 23.0f, 226.0f)
};
glm::vec3 _lightScale = glm::vec3(0.5f, 0.5f, 0.5f);
glm::vec3 _lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

//Lanterns
glm::vec3 _lanternPositions[numberOfPointlights] =
{
	glm::vec3(350.0f, -3.0f, 75.0f),
	glm::vec3(185.0f, 8.0f, 85.0f),
	glm::vec3(37.0f, 0.5f, 57.0f),
	glm::vec3(120.0f, -5.0f, 236.0f),
	glm::vec3(51.0f, 9.0f, 372.0f),
	glm::vec3(186.0f, 3.0f, 461.0f),
	glm::vec3(321.0f, 0.0f, 429.0f),
	glm::vec3(430.0f, 9.5f, 334.0f),
	glm::vec3(333.0f, 0.0f, 225.0f)
};
glm::vec3 _lanternScale = glm::vec3(3.0f, 3.0f, 3.0f);
