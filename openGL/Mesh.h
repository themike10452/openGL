#ifndef OPENGL_MESH_INCLUDED
#define OPENGL_MESH_INCLUDED

#include <vector>
#include "glm/glm.hpp"
#include "Program.h"

namespace openGL
{
	struct Model;

	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 UV;
	};

	struct Texture
	{
		GLuint Id;
		std::string Type;
		aiString Path;
	};

	struct Mesh
	{
		const std::vector<GLuint> Indices() const { return _indices; }
		const std::vector<openGL::Texture> Textures() const { return _textures; }
		const std::vector<openGL::Vertex>& Vertices() const { return _vertices; }

		Mesh(std::vector<openGL::Vertex>& vertices, std::vector<openGL::Texture>& textures, std::vector<GLuint>& indices)
			: _vertices(vertices), _textures(textures), _indices(indices)
		{
			this->SetupMesh();
		}

		void Draw() const
		{
			glBindVertexArray(_vertexArray);
			glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, nullptr);
			glBindVertexArray(0);
		}

	protected:
		GLuint _vertexBuffer, _indexBuffer, _vertexArray;

		std::vector<GLuint>& _indices;
		std::vector<openGL::Texture>& _textures;
		std::vector<openGL::Vertex>& _vertices;

		void SetupMesh()
		{
			glGenVertexArrays(1, &_vertexArray);
			glGenBuffers(1, &_vertexBuffer);
			glGenBuffers(1, &_indexBuffer);

			glBindVertexArray(_vertexArray);

			glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(openGL::Vertex) * _vertices.size(), &_vertices[0], GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * _indices.size(), &_indices[0], GL_STATIC_DRAW);

			glVertexAttribPointer(VertexAttribLocations::POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
			glEnableVertexAttribArray(VertexAttribLocations::POSITION);

			glVertexAttribPointer(VertexAttribLocations::NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
			glEnableVertexAttribArray(VertexAttribLocations::NORMAL);

			glVertexAttribPointer(VertexAttribLocations::UV, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, UV));
			glEnableVertexAttribArray(VertexAttribLocations::UV);

			glBindVertexArray(0);
		}
	};
}

#endif //!OPENGL_MESH_INCLUDED