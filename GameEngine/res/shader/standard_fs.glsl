#version 330 core

#define NR_OF_POINT_LIGHTS 9 

in vec2 texCoords_out;
in float visibility;
in vec4 worldPosition;
in vec3 normals_out;

out vec4 fragColor;

uniform sampler2D textureSampler;
uniform vec3 fogColor;
uniform vec3 lightColor;
uniform vec3 lightPositions[NR_OF_POINT_LIGHTS];
uniform vec3 viewPosition;

const float ambientStrength = 0.2;
const float diffuseStrength = 1.0;
const float specularStrength = 0.5;
const float lightConstant = 1.0;
const float lightLinear = 0.007;
const float lightQuadratic = 0.0002;
const float shininess = 16.0;

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
	//Texture
	vec4 texColor = texture(textureSampler, texCoords_out);

	//Beleuchtungsberechnung
	vec3 result;
	for (int i = 0; i < NR_OF_POINT_LIGHTS; i++)
	{
		result += PointLight(lightPositions[i], worldPosition, viewPosition);
	}

	//Verrechnung mit Bodenfarbe
	result *= vec3(texColor.xyz);

	//Fog (muss als letztes berechnet werden)
	vec4 mixColor = mix(vec4(fogColor, 1.0), vec4(result, 1.0), visibility);

	//Final-Fragmentcolor
	fragColor = mixColor;
}