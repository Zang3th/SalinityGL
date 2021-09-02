#version 450 core

layout(location = 0) in vec3 vertexIn;
layout(location = 1) in vec2 texCoordsIn;
layout(location = 2) in vec3 normalsIn;

out vec2 texCoords;
out vec3 normals;
out vec3 fragPos;
out vec4 fragPosLightSpace;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightProjection;

void main()
{
    texCoords = texCoordsIn;
    normals = transpose(inverse(mat3(model))) * normalsIn;
    fragPos = vec3(model * vec4(vertexIn, 1.0f));
    fragPosLightSpace = lightProjection * vec4(fragPos, 1.0);

    gl_Position = projection * view * vec4(fragPos, 1.0f);
}