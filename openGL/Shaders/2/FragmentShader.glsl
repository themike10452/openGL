#version 440 core

uniform sampler2D sampler_diffuse;

in vs_out
{
	vec3 FragPosition;
	vec3 Normal;
	vec2 UV;
} _vs_in;

out vec4 Color;

void main()
{
	Color = texture(sampler_diffuse, _vs_in.UV);
}