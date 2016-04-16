#ifndef OPENGL_COMMON_INCLUDED
#define OPENGL_COMMON_INCLUDED

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <assimp/types.h>
#include <vector>
#include <iostream>
#include <exception>

namespace openGL
{
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 UV;
		glm::vec3 Tangent;
		glm::vec3 Bitangent;
	};

	struct Texture
	{
		GLuint Id;
		GLenum Type;
		aiString Path;
	};
}

#endif //!OPENGL_COMMON_INCLUDED