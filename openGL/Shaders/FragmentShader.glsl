#version 330 core
#extension GL_ARB_explicit_uniform_location : require

layout (location = 20) uniform float SurfaceShininess;
layout (location = 21) uniform vec4 AmbientLight;
layout (location = 22) uniform vec3 DiffuseLight;
layout (location = 23) uniform vec3 SpecularLight;

layout (location = 24) uniform vec4 LightPosition;

in vs_out
{
	vec4 fragmentColor;
	vec4 fragmentPosition;
	vec4 normal;
} _vs_in;

out vec4 Color;

void main()
{
	vec4 lightDirection = normalize(LightPosition - _vs_in.fragmentPosition);

	//diffuse
	vec3 diffuseLight = vec3(1.0f, 1.0f, 1.0f);
	float diffIntensity = max(dot(_vs_in.normal, lightDirection), 0.0f);
	vec3 diffuse = diffIntensity * diffuseLight;

	Color = vec4(0.6f, 0.6f, 0.6f, 1.0f);
	Color = vec4((AmbientLight.rgb + diffuse) * Color.rgb, 1.0f);
}