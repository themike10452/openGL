#include "Mesh.h"
#include "assimp/material.h"

namespace openGL
{

	const std::vector<GLuint>& Mesh::Indices() const { return _indices; }
	const std::vector<openGL::Texture>& Mesh::Textures() const { return _textures; }
	const std::vector<openGL::Vertex>& Mesh::Vertices() const { return _vertices; }

	Mesh::Mesh(std::vector<openGL::Vertex> vertices, std::vector<GLuint> indices, std::vector<openGL::Texture> textures)
		: _vertices(vertices), _indices(indices), _textures(textures)
	{
		this->SetupMesh();
	}

	void Mesh::Draw(openGL::Program* program) const
	{
		const int texCount = _textures.size();
		GLint position;
		for (GLuint i = 0; i < texCount; i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			if (_textures[i].Type == aiTextureType_DIFFUSE)
			{
				program->SetUniform1i("sampler_diffuse", i);
			}
			else if (_textures[i].Type == aiTextureType_SPECULAR)
			{
				program->SetUniform1i("sampler_specular", i);
			}
			glBindTexture(GL_TEXTURE_2D, _textures[i].Id);
		}

		glBindVertexArray(_vertexArray);
		glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);

		for (GLuint i = 0; i < texCount; i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	void Mesh::SetupMesh()
	{
		glGenVertexArrays(1, &_vertexArray);
		glGenBuffers(1, &_vertexBuffer);
		glGenBuffers(1, &_indexBuffer);

		glBindVertexArray(_vertexArray);

		glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(openGL::Vertex) * _vertices.size(), &_vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * _indices.size(), &_indices[0], GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, UV));
		glEnableVertexAttribArray(2);

		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(openGL::Vertex), (void*)offsetof(openGL::Vertex, Tangent));
		glEnableVertexAttribArray(3);

		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(openGL::Vertex), (void*)offsetof(openGL::Vertex, Bitangent));
		glEnableVertexAttribArray(4);

		glBindVertexArray(0);
	}
}