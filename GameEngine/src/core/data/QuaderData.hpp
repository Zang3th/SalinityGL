#pragma once

#include "RawData.hpp"

class QuaderData : public RawData
{
private:
	glm::vec3 _camPosition, _endPosition;
	float _angle;

public:
	QuaderData()
		: _camPosition(10,10,10), _endPosition(100,10,10), _angle(20.0f)
	{
		updatePosition(_camPosition, _endPosition, _angle); //Erforderlich, da das Programm in DEBUG sonst crashed!
	}

	void updatePosition(const glm::vec3& camPosition, const glm::vec3& endPosition, const float& angle, const float& rayLength = 100.0f, const float& rayThickness = 0.25f)
	{
		//Preparation
		_camPosition = camPosition;
		_endPosition = endPosition;
		_angle = angle;

		_vertices.clear();
		_indices.clear();
		//-------------------------------------------------------------------Front Face-----------------------------------------------------------------------------------
		glm::vec3 originPosition(0, 0, 0);

		//Create face in origin
		glm::vec3 bottom_left = glm::vec3(originPosition.x, originPosition.y - rayThickness, originPosition.z - rayThickness);
		glm::vec3 bottom_right = glm::vec3(originPosition.x, originPosition.y - rayThickness, originPosition.z + rayThickness);
		glm::vec3 top_right = glm::vec3(originPosition.x, originPosition.y + rayThickness, originPosition.z + rayThickness);
		glm::vec3 top_left = glm::vec3(originPosition.x, originPosition.y + rayThickness, originPosition.z - rayThickness);

		//Create Transformationmatrix
		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::translate(trans, _camPosition);
		trans = glm::rotate(trans, glm::radians(_angle), glm::vec3(0, -1, 0));		

		//Create 4D Vector
		glm::vec4 bottom_left_4D = glm::vec4(bottom_left, 1.0f);
		glm::vec4 bottom_right_4D = glm::vec4(bottom_right, 1.0f);
		glm::vec4 top_right_4D = glm::vec4(top_right, 1.0f);
		glm::vec4 top_left_4D = glm::vec4(top_left, 1.0f);

		//Transform
		bottom_left_4D = trans * bottom_left_4D;
		bottom_right_4D = trans * bottom_right_4D;
		top_right_4D = trans * top_right_4D;
		top_left_4D = trans * top_left_4D;

		_vertices.emplace_back(glm::vec3(bottom_left_4D.x, bottom_left_4D.y, bottom_left_4D.z));
		_vertices.emplace_back(glm::vec3(bottom_right_4D.x, bottom_right_4D.y, bottom_right_4D.z));
		_vertices.emplace_back(glm::vec3(top_right_4D.x, top_right_4D.y, top_right_4D.z));
		_vertices.emplace_back(glm::vec3(top_left_4D.x, top_left_4D.y, top_left_4D.z));

		_indices.emplace_back(glm::uvec3(0, 1, 2));
		_indices.emplace_back(glm::uvec3(0, 2, 3));

		//-------------------------------------------------------------------Back Face-----------------------------------------------------------------------------------
		glm::vec3 calcEndPos = glm::vec3(_camPosition.x + _endPosition.x * rayLength, _camPosition.y + _endPosition.y * rayLength, _camPosition.z + _endPosition.z * rayLength);

		//Create face in origin
		bottom_left = glm::vec3(originPosition.x, originPosition.y - rayThickness, originPosition.z - rayThickness);
		bottom_right = glm::vec3(originPosition.x, originPosition.y - rayThickness, originPosition.z + rayThickness);
		top_right = glm::vec3(originPosition.x, originPosition.y + rayThickness, originPosition.z + rayThickness);
		top_left = glm::vec3(originPosition.x, originPosition.y + rayThickness, originPosition.z - rayThickness);

		//Create Transformationmatrix
		trans = glm::mat4(1.0f);
		trans = glm::translate(trans, calcEndPos);
		trans = glm::rotate(trans, glm::radians(_angle), glm::vec3(0, -1, 0));

		//Create 4D Vector
		bottom_left_4D = glm::vec4(bottom_left, 1.0f);
		bottom_right_4D = glm::vec4(bottom_right, 1.0f);
		top_right_4D = glm::vec4(top_right, 1.0f);
		top_left_4D = glm::vec4(top_left, 1.0f);

		//Transform
		bottom_left_4D = trans * bottom_left_4D;
		bottom_right_4D = trans * bottom_right_4D;
		top_right_4D = trans * top_right_4D;
		top_left_4D = trans * top_left_4D;

		_vertices.emplace_back(glm::vec3(bottom_left_4D.x, bottom_left_4D.y, bottom_left_4D.z));
		_vertices.emplace_back(glm::vec3(bottom_right_4D.x, bottom_right_4D.y, bottom_right_4D.z));
		_vertices.emplace_back(glm::vec3(top_right_4D.x, top_right_4D.y, top_right_4D.z));
		_vertices.emplace_back(glm::vec3(top_left_4D.x, top_left_4D.y, top_left_4D.z));

		_indices.emplace_back(glm::uvec3(4, 5, 6));
		_indices.emplace_back(glm::uvec3(4, 6, 7));

		//-------------------------------------------------------------------Other Faces----------------------------------------------------------------------------------
		//Bottom
		_indices.emplace_back(glm::uvec3(0, 1, 5));
		_indices.emplace_back(glm::uvec3(0, 5, 4));

		//Right
		_indices.emplace_back(glm::uvec3(1, 5, 6));
		_indices.emplace_back(glm::uvec3(1, 6, 2));

		//Top
		_indices.emplace_back(glm::uvec3(3, 2, 6));
		_indices.emplace_back(glm::uvec3(3, 6, 7));

		//Left
		_indices.emplace_back(glm::uvec3(0, 4, 7));
		_indices.emplace_back(glm::uvec3(0, 7, 3));		

		setParameters();
	}

	void setParameters()
	{
		_verticeSize = _vertices.size() * sizeof(glm::vec3);
		_indiceSize = _indices.size() * sizeof(glm::uvec3);
		_verticesToRender = (GLsizei)_indices.size() * 3;
	}
};