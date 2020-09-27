#version 440 core

layout(location = 0) in vec3 position_in;

out vec3 pos_out;

uniform mat4 view;
uniform mat4 projection;

void main()
{
	pos_out = position_in;
	vec4 pos = projection * view * vec4(position_in, 1.0);
	gl_Position = pos.xyww; //To fail the depth test
}