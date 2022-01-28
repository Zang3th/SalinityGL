#version 450 core

layout(location = 0) in vec3 vertexIn;
layout(location = 1) in vec2 texCoordsIn;
layout(location = 2) in vec3 normalsIn;

out vec2 texCoords;
out vec3 normals;
out vec4 fragPos;
out vec4 fragPosLightSpace;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightProjection;

void main()
{
    texCoords = texCoordsIn;
    normals = transpose(inverse(mat3(model))) * normalsIn;
    fragPos = model * vec4(vertexIn, 1.0f);
    fragPosLightSpace = lightProjection * fragPos;

    gl_Position = projection * view * fragPos;
}