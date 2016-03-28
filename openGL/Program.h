#ifndef OPENGL_PROGRAM_INCLUDED
#define OPENGL_PROGRAM_INCLUDED

#include <vector>
#include <GL\glew.h>
#include "ShaderDefinition.h"
#include "Utils.h"

namespace openGL
{
	enum VertexAttribLocations
	{
		POSITION = 0,
		COLOR = 1,
		NORMAL = 2,
		UV = 3,
	};

	enum VertexUniformLocations
	{
		AMBIENT_LIGHT = 10,
		MODEL_MATRIX = 20,
		VIEW_MATRIX = 21,
		PROJECTION_MATRIX = 22,
	};

	struct Program
	{
		GLuint Id() const { return _programId; }

		Program()
			: _programId(0)
		{
		};

		void Compile(std::vector<ShaderDefinition> &shaders)
		{
			std::vector<GLuint> compiledShaders;
			GLint status, logLength;

			int count = shaders.size();
			for (int i = 0; i < count; i++)
			{
				ShaderDefinition shaderDef = shaders.at(i);
				const char* filepath = shaderDef.FilePath();

				std::string shaderCode = Utils::ReadFile(filepath);
				const char* shaderCodePointer = shaderCode.c_str();
				GLuint shaderId = glCreateShader(shaderDef.Type());
				glShaderSource(shaderId, 1, &shaderCodePointer, nullptr);
				glCompileShader(shaderId);

				glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);
				glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength);

				if (logLength > 0)
				{
					std::vector<char> errorMessage(logLength + 1);
					glGetShaderInfoLog(shaderId, logLength, nullptr, &errorMessage[0]);
					printf("%s\n", &errorMessage[0]);
					if (status != GL_TRUE)
					{
						char* message = "Failed to compile shader: ";
						strcat_s(message, sizeof(filepath), filepath);
						throw std::exception(message);
					}
				}

				compiledShaders.push_back(shaderId);
			}

			_programId = glCreateProgram();

			for (int i = 0; i < count; i++)
			{
				glAttachShader(_programId, compiledShaders.at(i));
			}

			glLinkProgram(_programId);
			glGetProgramiv(_programId, GL_LINK_STATUS, &status);
			glGetProgramiv(_programId, GL_INFO_LOG_LENGTH, &logLength);

			if (logLength > 0)
			{
				std::vector<char> errorMessage(logLength + 1);
				glGetProgramInfoLog(_programId, logLength, nullptr, &errorMessage[0]);
				printf("%s\n", &errorMessage[0]);
				if (status != GL_TRUE) throw std::exception("Failed to link shader program");
			}

			for (int i = 0; i < count; i++)
			{
				glDetachShader(_programId, compiledShaders.at(i));
				glDeleteShader(compiledShaders.at(i));
			}
		}

		void Use() const
		{
			glUseProgram(_programId);
		}

		virtual ~Program()
		{
			if (_programId) glDeleteProgram(_programId);
		}

	protected:
		GLuint _programId;
	};
}

#endif // !OPENGL_PROGRAM_INCLUDED
