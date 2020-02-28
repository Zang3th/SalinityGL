#version 330 core

layout (location = 0) in vec3 position_in;
layout (location = 1) in vec2 texCoords_in;
layout (location = 2) in vec3 normals_in;

out vec2 texCoords_out;
out float visibility;
out vec4 worldPosition;
out vec3 normals_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

const float density = 0.0035;
const float gradient = 5.0;

void main()
{
	//MVP
	worldPosition = model * vec4(position_in, 1.0);
	vec4 positionToCam = view * worldPosition;
	gl_Position = projection * positionToCam;

	//Texturen
	texCoords_out = texCoords_in;

	//Fog
	float distance = length(positionToCam.xyz);
	visibility = exp(-pow((distance * density), gradient));
	visibility = clamp(visibility, 0.0, 1.0);

	//Normals
	//normals_out = normals_in;
	normals_out = mat3(transpose(inverse(model))) * normals_in;
}