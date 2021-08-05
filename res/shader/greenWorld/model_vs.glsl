#version 450 core

layout(location = 0) in vec3 vertexIn;
layout(location = 1) in vec2 texCoordsIn;
layout(location = 2) in vec3 normalsIn;

out vec2 texCoords;
out vec3 normals;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    texCoords = texCoordsIn;
    normals = mat3(transpose(inverse(model))) * normalsIn;
    vec4 fragPos = model * vec4(vertexIn, 1.0f);

    gl_Position = projection * view * vec4(vec3(fragPos), 1.0f);
}