#version 440 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

uniform vec3 ViewPos;
uniform vec3 SunPosition;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

out vs_out
{
	vec3 FragPos;
	vec3 Normal;
	vec2 UV;
	vec3 SunPos;
	vec3 TangentFragPos;
	vec3 TangentViewPos;
	vec3 TangentSunPos;
} _vs_out;

void main()
{
	mat3 normalMatrix = transpose(inverse(mat3(ModelMatrix)));
	vec3 T = normalize(normalMatrix * tangent);
	vec3 B = normalize(normalMatrix * bitangent);
	vec3 N = normalize(normalMatrix * normal);

	mat3 TBN = transpose(mat3(T, B, N));

	mat4 mvp = ProjectionMatrix * ViewMatrix * ModelMatrix;
	gl_Position =  mvp * vec4(position, 1.0);

	_vs_out.FragPos = vec3(ModelMatrix * vec4(position, 1.0f));
	_vs_out.Normal = TBN * normalize(vec3(ModelMatrix * vec4(normal, 0.0f)));
	_vs_out.UV = uv;
	_vs_out.SunPos = vec3(ModelMatrix * vec4(SunPosition, 1.0f));

	_vs_out.TangentFragPos = TBN * _vs_out.FragPos;
	_vs_out.TangentSunPos = TBN * _vs_out.SunPos;
	_vs_out.TangentViewPos = TBN * ViewPos;
}