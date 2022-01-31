#version 450 core

layout(location = 0) in vec3 vertexIn;
layout(location = 1) in vec2 texCoordsIn;

out vec2 texCoords;
out vec4 fragPos;
out vec4 clipSpace;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    texCoords = texCoordsIn;
    fragPos = model * vec4(vertexIn, 1.0f);
    clipSpace = projection * view * fragPos;
    gl_Position = clipSpace;
}