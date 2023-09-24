#version 450 core

layout(location = 0) in vec3 vertexIn;
layout(location = 1) in mat4 modelIn;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * modelIn * vec4(vertexIn, 1.0f);
}