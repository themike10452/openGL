#ifndef OPENGL_PROGRAM_INCLUDED
#define OPENGL_PROGRAM_INCLUDED

#include "Common.h"
#include "ShaderDefinition.h"
#include "Utils.h"

#include <vector>
#include <map>

namespace openGL
{
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

		GLuint GetUniformLocation(const char* name)
		{
			if (_uniformLocations.count(name))
			{
				return _uniformLocations[name];
			}
			else
			{
				GLuint location = glGetUniformLocation(_programId, name);
				_uniformLocations[name] = location;
				return location;
			}
		}

		void SetUniform1i(const char* name, int value)
		{
			SetUniform1i(this->GetUniformLocation(name), value);
		}

		void SetUniform1i(GLuint location, int value) const
		{
			glUniform1i(location, value);
		}

		void SetUniform1f(const char* name, float value)
		{
			SetUniform1f(this->GetUniformLocation(name), value);
		}

		void SetUniform1f(GLuint location, float value) const
		{
			glUniform1f(location, value);
		}

		void SetUniform2fv(const char* name, const GLfloat* value)
		{
			this->SetUniform2fv(this->GetUniformLocation(name), value);
		}

		void SetUniform2fv(GLuint location, const GLfloat* value) const
		{
			glUniform2fv(location, 1, value);
		}

		void SetUniform3fv(const char* name, const GLfloat* value)
		{
			this->SetUniform3fv(this->GetUniformLocation(name), value);
		}

		void SetUniform3fv(GLuint location, const GLfloat* value) const
		{
			glUniform3fv(location, 1, value);
		}

		void SetUniform4fv(const char* name, const GLfloat* value)
		{
			this->SetUniform4fv(this->GetUniformLocation(name), value);
		}

		void SetUniform4fv(GLuint location, const GLfloat* value) const
		{
			glUniform4fv(location, 1, value);
		}

		void SetUniformMat3fv(const char* name, const GLfloat* value)
		{
			this->SetUniformMat3fv(this->GetUniformLocation(name), value);
		}

		void SetUniformMat3fv(GLuint location, const GLfloat* value) const
		{
			glUniformMatrix3fv(location, 1, GL_FALSE, value);
		}

		void SetUniformMat4fv(const char* name, const GLfloat* value)
		{
			this->SetUniformMat4fv(this->GetUniformLocation(name), value);
		}

		void SetUniformMat4fv(GLuint location, const GLfloat* value) const
		{
			glUniformMatrix4fv(location, 1, GL_FALSE, value);
		}

		virtual ~Program()
		{
			if (_programId) glDeleteProgram(_programId);
		}

	protected:
		GLuint _programId;

		typedef std::map<std::string, GLint> StringIntegerMap;
		StringIntegerMap _uniformLocations;
	};
}

#endif // !OPENGL_PROGRAM_INCLUDED
