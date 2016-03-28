#ifndef OPENGL_UTILS_INCLUDED
#define OPENGL_UTILS_INCLUDED

#include <fstream>
#include <sstream>
#include "SOIL/SOIL.h"

namespace openGL
{
	struct Utils
	{
		static std::string ReadFile(const char* filepath)
		{
			std::fstream stream(filepath, std::ios::in);
			if (!stream.is_open())
			{
				char* errorMessage = "Failed to read from file: ";
				strcat_s(errorMessage, sizeof(filepath), filepath);
				throw std::exception(errorMessage);
			}
			std::stringstream ss;
			ss << stream.rdbuf();
			stream.close();
			return ss.str();
		}

		static GLuint TextureFromFile(const char* filename, std::string directory)
		{
			std::string filepath = std::string(filename) + directory;
			GLuint textureId;
			glGenTextures(1, &textureId);
			int width, height;
			unsigned char* image = SOIL_load_image(filepath.c_str(), &width, &height, nullptr, SOIL_LOAD_RGBA);

			//assign image data to texture
			glBindTexture(GL_TEXTURE_2D, textureId);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
			glGenerateMipmap(GL_TEXTURE_2D);

			//set some parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glBindTexture(GL_TEXTURE_2D, 0);
			SOIL_free_image_data(image);
			return textureId;
		}
	};
}

#endif // !OPENGL_UTILS_INCLUDED
