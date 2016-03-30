#version 330 core
#extension GL_ARB_explicit_uniform_location : require

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 normal;
layout (location = 2) in vec2 uv;
layout (location = 3) in vec4 color;

layout (location = 10) uniform mat4 ModelMatrix;
layout (location = 11) uniform mat4 ViewMatrix;
layout (location = 12) uniform mat4 ProjectionMatrix;

out vs_out
{
	vec4 fragmentColor;
	vec4 fragmentPosition;
	vec4 normal;
} _vs_out;

void main()
{
	mat4 mvp = ProjectionMatrix * ViewMatrix * ModelMatrix;
	gl_Position =  mvp * position;

	_vs_out.fragmentColor = color;
	_vs_out.fragmentPosition = vec4(ModelMatrix * position);
	_vs_out.normal = normalize(normal);
}