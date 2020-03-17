#pragma once

class MousePicker
{
private:
	glm::mat4 _projection;
	glm::mat4 _view;
	GroundData* _terrain;
	
public:
	glm::vec3 _mouseRay = glm::vec3(0, 0, 0);
	glm::vec3 _mouseRayTerrainEntry = glm::vec3(0, 0, 0);
	
	MousePicker(GroundData* Terrain)
		: _terrain(Terrain)
	{

	}

	void update()
	{
		_projection = glm::perspective(glm::radians(_camera->Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 10000.0f);
		_view = _camera->GetViewMatrix();
		_mouseRay = calculateMouseRay();
	}

	glm::vec3 calculateMouseRay()
	{
		//3d Normalised Device Coordinates
		float x = (2.0f * rawMouse_X) / (float)WIDTH - 1.0f;
		float y = 1.0f - (2.0f * rawMouse_Y) / (float)HEIGHT;
		glm::vec3 ray_NDS = glm::vec3(x, y, 1.0f);

		//4d Homogeneous Clip Coordinates
		glm::vec4 ray_CLIP = glm::vec4(ray_NDS.x, ray_NDS.y, -1.0, 1.0);

		//4d Eye (Camera) Coordinates
		glm::vec4 ray_EYE = glm::inverse(_projection) * ray_CLIP;
		ray_EYE = glm::vec4(ray_EYE.x, ray_EYE.y, -1.0, 0.0);

		//3d World Coordinates
		glm::vec4 ray_WORLD_4D = glm::inverse(_view) * ray_EYE;
		glm::vec3 ray_WORLD_3D = glm::vec3(ray_WORLD_4D.x, ray_WORLD_4D.y, ray_WORLD_4D.z);
		ray_WORLD_3D = glm::normalize(ray_WORLD_3D);

		return ray_WORLD_3D;
	}

	void calculateTerrainEntry(const glm::vec3& camPosition, const glm::vec3 endPosition)
	{
		int rayLength = 1000;
		float x, y, z;

		for (int i = 0; i < 10000; i++)
		{
			x = camPosition.x + endPosition.x * rayLength;
			y = camPosition.y + endPosition.y * rayLength;
			z = camPosition.z + endPosition.z * rayLength;
			
			float terrain_height = _terrain->getHeightValueUnbuffered(x, z);

			if (terrain_height > y)
			{
				rayLength -= 0.1f;
			}
			else if (terrain_height < y)
			{
				rayLength += 0.1f;
			}
			else if (terrain_height == y)
			{
				break;
			}			
		}

		_mouseRayTerrainEntry = glm::vec3(x, y - 0.5, z);
	}
};