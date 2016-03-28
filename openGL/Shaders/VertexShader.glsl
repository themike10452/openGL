#version 330 core
#extension GL_ARB_explicit_uniform_location : require

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec4 normal;
layout (location = 3) in vec2 uv;

layout (location = 20) uniform mat4 ModelMatrix;
layout (location = 21) uniform mat4 ViewMatrix;
layout (location = 22) uniform mat4 ProjectionMatrix;

out vs_out
{
	vec4 fragmentColor;
} vertex_shader_output;

void main()
{
	mat4 mvp = ProjectionMatrix * ViewMatrix * ModelMatrix;
	gl_Position =  mvp * position;
	vertex_shader_output.fragmentColor = color;
}