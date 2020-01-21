#version 330 core

layout (location = 0) in vec3 position_in;
layout (location = 1) in vec2 texCoords_in;
layout (location = 2) in vec3 terrainColors_in;

out vec3 color_out;
out vec2 texCoords_out;
flat out int type_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(position_in, 1.0);
	texCoords_out = texCoords_in;
	if (terrainColors_in.x == 255) {
		color_out = vec3(0.972, 0.929, 0.447);
		type_out = 0;
	}
	else if(terrainColors_in.y == 255){
		color_out = vec3(position_in.y / 22 + 0.55, position_in.y / 22 + 0.75, position_in.y / 22 + 0.55);
		type_out = 1;
	}	
	else {
		color_out = vec3(0.521, 0.521, 0.521);
		type_out = 2;
	}
}