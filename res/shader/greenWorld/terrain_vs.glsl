#version 450 core

layout(location = 0) in vec3 vertexIn;
layout(location = 1) in vec2 texCoordsIn;
layout(location = 2) in vec3 normalsIn;

out vec2 texCoords;
out vec2 texCoordsTiled;
out vec3 normals;
out vec4 fragPos;
out vec4 fragPosLightSpace;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightProjection;
uniform vec4 clipPlane;

void main()
{
    texCoords = texCoordsIn / 128.0f;
    texCoordsTiled = texCoordsIn;
    normals = transpose(inverse(mat3(model))) * normalsIn;
    fragPos = model * vec4(vertexIn, 1.0f);
    fragPosLightSpace = lightProjection * fragPos;
    gl_ClipDistance[0] = dot(fragPos, clipPlane);

    gl_Position = projection * view * fragPos;
}