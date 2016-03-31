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
		NORMAL = 1,
		UV = 2,
		COLOR = 3,
	};

	enum VertexUniformLocations
	{
		MODEL_MATRIX = 10,
		VIEW_MATRIX = 11,
		PROJECTION_MATRIX = 12
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
					if (status != GL_TRUE)
					{
						std::stringstream ss;
						ss << "Failed to compile shader: " << filepath << std::endl;
						ss << &errorMessage[0];
						std::string log = ss.str();
						printf("%s\n", log.c_str());
						throw std::exception(log.c_str());
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

			if (logLength > 0 && status != GL_TRUE)
			{
				std::vector<char> errorMessage(logLength + 1);
				glGetProgramInfoLog(_programId, logLength, nullptr, &errorMessage[0]);
				std::stringstream ss;
				ss << "Failed to link shader program" << std::endl;
				ss << &errorMessage[0];
				std::string message = ss.str();
				printf("%s\n", message.c_str());
				throw std::exception(message.c_str());
			}

			for (int i = 0; i < count; i++)
			{
				glDetachShader(_programId, compiledShaders.at(i));
				glDeleteShader(compiledShaders.at(i));
			}
		}

		void Bind() const
		{
			glUseProgram(_programId);
		}

		void Unbind() const
		{
			glUseProgram(0);
		}

		GLint GetUniformLocation(const char* name)
		{
			for (Uniform u : _uniforms)
			{
				if (u.Name == name)
				{
					return u.Location;
				}
			}

			GLint location = glGetUniformLocation(_programId, name);;
			_uniforms.push_back(Uniform(name, location));
			return location;
		}

		void SetUniform1f(const char* name, float value)
		{
			SetUniform1f(this->GetUniformLocation(name), value);
		}

		void SetUniform1f(GLint location, float value) const
		{
			Bind();
			glUniform1f(location, value);
			Unbind();
		}

		void SetUniform2fv(const char* name, const GLfloat* value)
		{
			this->SetUniform2fv(this->GetUniformLocation(name), value);
		}

		void SetUniform2fv(GLint location, const GLfloat* value) const
		{
			Bind();
			glUniform2fv(location, 1, value);
			Unbind();
		}

		void SetUniform3fv(const char* name, const GLfloat* value)
		{
			this->SetUniform3fv(this->GetUniformLocation(name), value);
		}

		void SetUniform3fv(GLint location, const GLfloat* value) const
		{
			Bind();
			glUniform3fv(location, 1, value);
			Unbind();
		}

		void SetUniform4fv(const char* name, const GLfloat* value)
		{
			this->SetUniform4fv(this->GetUniformLocation(name), value);
		}

		void SetUniform4fv(GLint location, const GLfloat* value) const
		{
			Bind();
			glUniform4fv(location, 1, value);
			Unbind();
		}

		void SetUniformMat3fv(const char* name, const GLfloat* value)
		{
			this->SetUniformMat3fv(this->GetUniformLocation(name), value);
		}

		void SetUniformMat3fv(GLint location, const GLfloat* value) const
		{
			Bind();
			glUniformMatrix3fv(location, 1, GL_FALSE, value);
			Unbind();
		}

		void SetUniformMat4fv(const char* name, const GLfloat* value)
		{
			this->SetUniformMat4fv(this->GetUniformLocation(name), value);
		}

		void SetUniformMat4fv(GLint location, const GLfloat* value) const
		{
			Bind();
			glUniformMatrix4fv(location, 1, GL_FALSE, value);
			Unbind();
		}

		virtual ~Program()
		{
			if (_programId) glDeleteProgram(_programId);
		}

	protected:
		struct Uniform
		{
			const char* Name;
			GLint Location;

			Uniform(const char* name, GLint location)
				: Name(name), Location(location) {}
		};

		GLuint _programId;
		std::vector<Uniform> _uniforms;
	};
}

#endif // !OPENGL_PROGRAM_INCLUDED
