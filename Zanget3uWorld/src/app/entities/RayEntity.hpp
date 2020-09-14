#pragma once

#include "QuaderData.hpp"
#include "Shader.hpp"
#include "Primitivemodel.hpp"

class RayEntity
{
private:
	QuaderData* _quaderData;
	Shader* _shader;
	Primitivemodel* _quaderModel;
	friend class EntityManager;
	
	void updateRay(const glm::vec3& camPosition, const glm::vec3& endPosition, const float& angle, bool& renderRay, const float& rayLength, const float& rayThickness) const
	{		
		if (renderRay == false)
		{
			_quaderModel->renderModel = false;
		}			
		else
		{
			_quaderData->updatePosition(camPosition, endPosition, angle, rayLength, rayThickness);
			_quaderModel->updateData(_quaderData);
			_quaderModel->renderModel = true;
		}			
	}
	
public:
	RayEntity()
	{
		//Create the data
		_quaderData = new QuaderData();

		//Create the shader
		_shader = new Shader("../res/shader/zanget3uWorld/primitive_vs.glsl", "../res/shader/zanget3uWorld/primitive_fs.glsl");

		//Combine everything to the model
		_quaderModel = new Primitivemodel(_quaderData, _shader);
		_quaderModel->renderModel = false;
	}

	~RayEntity()
	{
		delete _quaderData;
		delete _shader;
		delete _quaderModel;
	}	
};