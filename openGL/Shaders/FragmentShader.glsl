#version 330 core
#extension GL_ARB_explicit_uniform_location : require

layout (location = 10) uniform vec4 AmbientLight;

in vs_out
{
	vec4 fragmentColor;
} vertex_shader_output;

out vec4 Color;

void main()
{
	//Color = vertex_shader_output.fragmentColor * AmbientLight;
	Color = vec4(0.0f, 0.6f, 0.8f, 1.0f);// * AmbientLight;
}