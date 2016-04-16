#version 440 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

out vs_out
{
	vec3 FragPosition;
	vec3 Normal;
	vec2 UV;
} _vs_out;

void main()
{
	mat4 mvp = ProjectionMatrix * ViewMatrix * ModelMatrix;
	gl_Position =  mvp * vec4(position, 1.0);

	//_vs_out.FragPosition = vec3(mat3(ModelMatrix) * position);
	//_vs_out.Normal = normalize(vec3(mat3(transpose(inverse(ModelMatrix))) * normal));
	_vs_out.UV = uv;
}