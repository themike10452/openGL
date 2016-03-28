#ifndef OPENGL_SHADER_DEFINITION_INCLUDED
#define OPENGL_SHADER_DEFINITION_INCLUDED
#include <GL\glew.h>

namespace openGL
{
	struct ShaderDefinition
	{
		const char* FilePath() const { return _filepath; }
		GLenum Type() const { return _type; }

		ShaderDefinition(const char* filepath, GLenum shadertype)
			: _filepath(filepath), _type(shadertype)
		{
		}

	protected:
		const char* _filepath;
		GLenum _type;
	};
}

#endif // !OPENGL_SHADER_DEFINITION_INCLUDED