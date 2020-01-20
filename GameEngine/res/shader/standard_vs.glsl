#version 330 core

layout (location = 0) in vec3 position_in;
layout (location = 1) in vec2 texCoords_in;

out vec3 color_out;
out vec2 texCoords_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(position_in, 1.0);
	texCoords_out = texCoords_in;
	color_out = vec3(1.0);
}