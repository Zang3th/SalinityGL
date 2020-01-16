#version 330 core

layout (location = 0) in vec3 position_in;
layout (location = 1) in vec2 texCoords_in;

out vec3 color_out;
out vec2 texCoords_out;

void main()
{
	gl_Position = vec4(position_in, 1.0);
	color_out = vec3(position_in.x + 0.5, 1.0, position_in.y + 0.5);
	texCoords_out = texCoords_in;
}