#version 330 core

#define NR_OF_POINT_LIGHTS 9  

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
uniform vec3 fogColor;
uniform vec3 lightColor;
uniform vec3 lightPositions[NR_OF_POINT_LIGHTS];
uniform vec3 viewPosition;

const float ambientStrength = 0.2;
const float diffuseStrength = 0.8;
const float specularStrength = 0.4;
const float lightConstant = 1.0;
const float lightLinear = 0.007;
const float lightQuadratic = 0.0002;
const float shininess = 4.0;

//----------------Beleuchtung----------------
vec3 PointLight(vec3 lightPosition, vec4 worldPosition, vec3 viewPosition) 
{
	//Attenuation
	float distance = length(lightPosition - vec3(worldPosition.xyz));
	float attenuation = 1.0 / (lightConstant + lightLinear * distance + lightQuadratic * (distance * distance));

	//Ambient
	vec3 ambientLight = ambientStrength * lightColor * attenuation;

	//Diffuse
	vec3 Normal = normalize(normals_out);
	vec3 lightDir = normalize(-(lightPosition - vec3(worldPosition.xyz)));
	float diff = max(dot(normals_out, lightDir), 0.0);
	vec3 diffuseLight = diff * diffuseStrength * lightColor * attenuation;

	//Specular
	vec3 viewDir = normalize(viewPosition - vec3(worldPosition.xyz));
	vec3 reflectDir = reflect(lightDir, Normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	vec3 specularLight = specularStrength * spec * lightColor * attenuation;

	return (ambientLight + diffuseLight + specularLight);
}

void main()
{
	//Texturen (Grass, Feldweg)
	vec4 blendmapColor = texture(blendmap, blendmapCoords_out);
	vec4 rTexture = texture(dirtTexture, texCoords_out) * blendmapColor.r * 0.5;
	vec4 gTexture = texture(grassTexture, texCoords_out) * blendmapColor.g * vec4(heightcolor_out, 1.0);
	vec4 bTexture = texture(stoneTexture, texCoords_out) * blendmapColor.b;
	vec4 groundColor = (rTexture + gTexture + bTexture);

	//Beleuchtungsberechnung
	vec3 result;	
	for (int i = 0; i < NR_OF_POINT_LIGHTS; i++) 
	{
		result += PointLight(lightPositions[i], worldPosition, viewPosition);
	}		

	//Verrechnung mit Bodenfarbe
	result *= vec3(groundColor.xyz);

	//Fog (muss als letztes berechnet werden)
	vec4 mixColor = mix(vec4(fogColor, 1.0), vec4(result, 1.0), visibility);

	//Final-Fragmentcolor
	fragColor = mixColor;
}