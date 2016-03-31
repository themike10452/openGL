#version 330 core
#extension GL_ARB_explicit_uniform_location : require

struct Light
{
	vec3 Color;
	vec3 Position;
	float Attenuation;
};

uniform Light AmbientLight;

uniform Light Lights[2];

in vs_out
{
	vec4 FragColor;
	vec3 FragPosition;
	vec3 Normal;
} _vs_in;

out vec4 Color;

void main()
{
	Color = vec4(0.6f, 0.6f, 0.6f, 1.0f);

	vec3 diffuseLights = vec3(0.0f, 0.0f, 0.0f);

	for(int i = 0; i < Lights.length(); i++)
	{
		vec3 lightDirection = normalize(Lights[i].Position - _vs_in.FragPosition);
		float diffIntensity = max(dot(_vs_in.Normal, lightDirection), 0.0f);
		diffuseLights += diffIntensity * Lights[i].Color;
	}

	Color = vec4((AmbientLight.Color + diffuseLights) * Color.rgb, 1.0f);
}