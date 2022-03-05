#version 450 core

layout(location = 0) in vec3 vertexIn;
layout(location = 1) in vec2 texCoordsIn;
layout(location = 2) in vec3 normalsIn;
layout(location = 3) in vec3 tangentsIn;

out vec2 texCoords;
out vec3 normals;
out vec3 fragPos;
out vec4 fragPosLightSpace;
out vec3 tangents;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightProjection;
uniform vec4 clipPlane;

void main()
{
    texCoords           = texCoordsIn;
    vec4 worldPos       = model * vec4(vertexIn, 1.0f);
    fragPos             = worldPos.xyz;
    fragPosLightSpace   = lightProjection * worldPos;
    gl_ClipDistance[0]  = dot(worldPos, clipPlane);

    //Transform tangents and normals into world space
    tangents = vec3(model * vec4(tangentsIn, -1.0));
    normals = vec3(model * vec4(normalsIn, 0.0));

    gl_Position         = projection * view * worldPos;
}