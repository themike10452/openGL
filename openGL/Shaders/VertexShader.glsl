#version 330 core
#extension GL_ARB_explicit_uniform_location : require

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;
layout (location = 3) in vec4 color;

layout (location = 10) uniform mat4 ModelMatrix;
layout (location = 11) uniform mat4 ViewMatrix;
layout (location = 12) uniform mat4 ProjectionMatrix;

out vs_out
{
	vec4 FragColor;
	vec3 FragPosition;
	vec3 Normal;
} _vs_out;

void main()
{
	mat4 mvp = ProjectionMatrix * ViewMatrix * ModelMatrix;
	gl_Position =  mvp * vec4(position, 1.0);

	_vs_out.FragColor = color;
	_vs_out.FragPosition = vec3(mat3(ModelMatrix) * position);
	_vs_out.Normal = normalize(vec3(mat3(transpose(inverse(ModelMatrix))) * normal));
}