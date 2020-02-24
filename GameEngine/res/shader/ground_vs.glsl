#version 330 core

layout (location = 0) in vec3 position_in;
layout (location = 1) in vec2 texCoords_in;
layout (location = 2) in vec2 blendmapCoords_in;

out vec3 heightcolor_out;
out vec2 texCoords_out;
out vec2 blendmapCoords_out;
out float visibility;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

const float density = 0.0035;
const float gradient = 5.0;

void main()
{
	//MVP
	vec4 worldPosition = model * vec4(position_in, 1.0);
	vec4 positionToCam = view * worldPosition;
	gl_Position = projection * positionToCam;

	//Texturen (Grass, Feldweg)
	texCoords_out = texCoords_in;
	blendmapCoords_out = blendmapCoords_in;
	heightcolor_out = vec3(position_in.y / 20 + 0.65, position_in.y / 20 + 0.75, position_in.y / 20 + 0.65);

	//Fog
	float distance = length(positionToCam.xyz);
	visibility = exp(-pow((distance * density), gradient));
	visibility = clamp(visibility, 0.0, 1.0);
}