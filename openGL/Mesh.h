#ifndef OPENGL_MESH_INCLUDED
#define OPENGL_MESH_INCLUDED

#include "Common.h"
#include "Program.h"

#include <vector>

namespace openGL
{
	struct Model;

	struct Mesh
	{
		/// Getters
		const std::vector<GLuint>& Indices() const;
		const std::vector<openGL::Texture>& Textures() const;
		const std::vector<openGL::Vertex>& Vertices() const;
		/// -------

		Mesh(std::vector<openGL::Vertex> vertices, std::vector<GLuint> indices, std::vector<openGL::Texture> textures);

		void Draw(openGL::Program* program) const;

	protected:
		GLuint _vertexBuffer, _indexBuffer, _vertexArray;

		std::vector<openGL::Vertex> _vertices;
		std::vector<GLuint> _indices;
		std::vector<openGL::Texture> _textures;

		void SetupMesh();
	};
}

#endif //!OPENGL_MESH_INCLUDED