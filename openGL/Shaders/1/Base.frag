#version 440 core

struct Light
{
	vec3 Color;
	float Attenuation;
};

struct PointLight
{
	vec3 Color;
	float Attenuation;
	vec3 Position;
};

uniform sampler2D sampler_diffuse;
uniform sampler2D sampler_specular;
uniform Light AmbientLight;

in vs_out
{
	vec3 FragPos;
	vec3 Normal;
	vec2 UV;
	vec3 SunPos;
	vec3 TangentFragPos;
	vec3 TangentViewPos;
	vec3 TangentSunPos;
} _vs_in;

out vec4 Color;

//prototypes
vec3 CalculateFragColor(vec3 lightPos, vec3 fragPos, vec3 normal, vec2 texCoords, vec3 viewDir);

void main()
{
	vec3 viewDir = normalize(_vs_in.TangentViewPos - _vs_in.TangentFragPos);
	vec3 fragColor = CalculateFragColor(_vs_in.TangentSunPos, _vs_in.TangentFragPos, _vs_in.Normal, _vs_in.UV, viewDir);

	Color = vec4(fragColor, 1.0f);
}

vec3 CalculateFragColor(vec3 lightPos, vec3 fragPos, vec3 normal, vec2 texCoords, vec3 viewDir)
{
	vec3 lightColor = vec3(0.8f);

	vec3 lightDir = normalize(lightPos - fragPos);
	//diffuse shading
	float diffAmount = max(dot(normal, lightDir), 0.0f);
	//specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float specAmount = pow(max(dot(viewDir, reflectDir), 0.0f), 32.0f);

	vec3 ambient = AmbientLight.Color * vec3(texture(sampler_diffuse, texCoords));
	vec3 diffuse = diffAmount * lightColor * vec3(texture(sampler_diffuse, texCoords));
	vec3 specular = specAmount * vec3(0.05f) * vec3(texture(sampler_specular, texCoords));

	return ambient + diffuse + specular;
}