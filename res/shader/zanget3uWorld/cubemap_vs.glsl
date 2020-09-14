#version 330 core

layout(location = 0) in vec3 position_in;

out vec3 pos_out;

uniform mat4 view;
uniform mat4 projection;

void main()
{
	pos_out = position_in;
	gl_Position = projection * view * vec4(position_in, 1.0);
}