#pragma once

#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"

class Basemodel
{
public:
	VertexArray* _vao = nullptr;
	VertexBuffer* _vbo1 = nullptr;
	VertexBuffer* _vbo2 = nullptr;
	IndexBuffer* _ib = nullptr;	
	glm::mat4 _model = glm::mat4(1.0f);
	glm::mat4 _projection;
	glm::mat4 _view;	

	void virtual initialize() = 0;
	void virtual draw() = 0;
	void virtual undraw() = 0;
	void virtual translate(const glm::vec3& position) = 0;
	void virtual rotate(const float& angle, const glm::vec3& axis) = 0;
	void virtual scale(const glm::vec3& scalar) = 0;
	unsigned int virtual getNumberOfVertices() = 0;
};