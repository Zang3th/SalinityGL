#version 450 core

layout (location = 0) in vec3 vertexIn;

uniform mat4 lightProjection;
uniform mat4 model;

void main()
{
    gl_Position = lightProjection * model * vec4(vertexIn, 1.0);
}