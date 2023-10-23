#version 450 core

layout(location = 0) in vec3 vertexIn;
layout(location = 1) in vec3 normalsIn;
layout(location = 2) in vec3 posIn;
layout(location = 3) in vec3 colorIn;

out vec3 color;
out vec3 normals;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    color = colorIn;

    //Transform normals and fragPos into world space
    normals = vec3(model * vec4(normalsIn, 0.0));
    fragPos = vec3(model * vec4(vertexIn, 1.0f));

    gl_Position = projection * view * model * vec4(vertexIn + posIn, 1.0f);
}