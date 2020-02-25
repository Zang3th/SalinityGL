#version 330 core

in vec3 heightcolor_out;
in vec2 texCoords_out;
in vec2 blendmapCoords_out;
in float visibility;
in vec3 normals_out;
in vec4 worldPosition;

out vec4 fragColor;

uniform sampler2D grassTexture;
uniform sampler2D dirtTexture;
uniform sampler2D stoneTexture;
uniform sampler2D blendmap;
uniform vec3 skyColor;
uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 viewPosition;

const float ambientStrength = 0.3;
const float diffuseStrength = 0.6;
const float specularStrength = 0.8;

void main()
{
	//Texturen (Grass, Feldweg)
	vec4 blendmapColor = texture(blendmap, blendmapCoords_out);
	vec4 rTexture = texture(dirtTexture, texCoords_out) * blendmapColor.r * 0.5;
	vec4 gTexture = texture(grassTexture, texCoords_out) * blendmapColor.g * vec4(heightcolor_out, 1.0);
	vec4 bTexture = texture(stoneTexture, texCoords_out) * blendmapColor.b;
	vec4 groundColor = (rTexture + gTexture + bTexture);	

	//----------------Beleuchtung----------------
	//Ambient
	vec3 ambientLight = ambientStrength * lightColor;

	//Diffuse
	vec3 Normal = normalize(normals_out);
	vec3 lightDir = normalize(-(lightPosition - vec3(worldPosition.xyz)));
	float diff = max(dot(normals_out, lightDir), 0.0);
	vec3 diffuseLight = diff * diffuseStrength * lightColor;

	//Specular
	vec3 viewDir = normalize(viewPosition - vec3(worldPosition.xyz));
	vec3 reflectDir = reflect(-lightDir, Normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
	vec3 specular = specularStrength * spec * lightColor;

	//Ergebnis der Beleuchtung
	vec3 result = (ambientLight + diffuseLight + specular) * vec3(groundColor.xyz);
	//-------------------------------------------

	//Fog (muss als letztes berechnet werden)
	vec4 mixColor = mix(vec4(skyColor, 1.0), vec4(result, 1.0), visibility);

	//Final-Fragmentcolor
	fragColor = vec4(mixColor);
}