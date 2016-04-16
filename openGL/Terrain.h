#ifndef OPENGL_TERRAIN_INCLUDED
#define OPENGL_TERRAIN_INCLUDED
#define MAX_HEIGHT 300.f

#include "Common.h"
#include "Interfaces.h"
#include "Utils.h"

#include <SDL/SDL.h>
#include <vector>

namespace openGL
{
	struct Terrain : public IComponent
	{
		float GetTerrainHeight(glm::vec3& position)
		{
			return this->GetTerrainHeight(position.x, position.z);
		}

		float GetTerrainHeight(float x, float z)
		{
			x *= (float)_heightmapWidth / _terrainWidth;
			z *= (float)_heightmapHeight / _terrainHeight;

			int index = floor(z) * _heightmapWidth + floor(x);
			if (index >= 0 && index < _vertices.size())
			{
				return _vertices[index].Position.y;
			}
			else
			{
				return -MAXINT;
			}
		}

		Terrain()
		{
		};

		explicit Terrain(int width, int height, const char* heightMap, const char* textureMap)
		{
			this->LoadHeightMap(width, height, heightMap);
			this->LoadTexture(textureMap);
		}

		void LoadHeightMap(int width, int height, const char* heightMap)
		{
			_terrainWidth = width; _terrainHeight = height;

			SDL_Surface* surface = SDL_LoadBMP(heightMap);

			_heightmapWidth = surface->w;
			_heightmapHeight = surface->h;

			UINT32 pixel;
			UINT8 r, g, b;

			_vertices.clear();
			_vertices.reserve(_heightmapWidth * _heightmapHeight);

			for (int i = 0; i < _heightmapWidth; i++)
			{
				for (int j = 0; j < _heightmapHeight; j++)
				{
					pixel = ((UINT32*)surface->pixels)[i * _heightmapWidth + j];
					SDL_GetRGB(pixel, surface->format, &r, &g, &b);
					openGL::Vertex vertex;
					vertex.Position = glm::vec3((float)j / _heightmapHeight * height, (g / 255.0f - 0.5f) * MAX_HEIGHT, (float)i / _heightmapWidth * width);
					vertex.UV = glm::vec2((float)j / _heightmapWidth, (float)i / _heightmapHeight);
					_vertices.push_back(vertex);
				}
			}

			openGL::Vertex previous_x, previous_z, next_x, next_z;
			glm::vec3 normal, tangent, bitangent;

			int lt, rt, lb, rb;
			for (int i = 0; i < _heightmapHeight - 1; i++)
			{
				for (int j = 0; j < _heightmapWidth - 1; j++)
				{
					lt = i * _heightmapWidth + j;
					rt = lt + 1;
					lb = lt + _heightmapWidth;
					rb = lb + 1;

					_indices.push_back(rt);
					_indices.push_back(lt);
					_indices.push_back(lb);

					_indices.push_back(rt);
					_indices.push_back(lb);
					_indices.push_back(rb);

					if (i > 0 && j > 0)
					{
						previous_x = _vertices[lt - 1];
						previous_z = _vertices[lt - _heightmapWidth];
						next_x = _vertices[rt];
						next_z = _vertices[lb];

						tangent = glm::normalize(next_x.Position - previous_x.Position);
						bitangent = glm::normalize(next_z.Position - previous_z.Position);
						normal = glm::normalize(glm::cross(bitangent, tangent));

						_vertices[lt].Tangent = tangent;
						_vertices[lt].Bitangent = bitangent;
						_vertices[lt].Normal = normal;

						_vertices[lt].Position.y = (next_x.Position.y + next_z.Position.y + previous_x.Position.y + previous_z.Position.y) / 4.0f;
					}
				}
			}
		}

		void LoadTexture(const char* textureMap)
		{
			_textureId = Utils::TextureFromFile(textureMap, "");
		}

		void Initialize() override
		{
			glGenVertexArrays(1, &_vertexArray);
			glGenBuffers(1, &_vertexBuffer);
			glGenBuffers(1, &_indexBuffer);

			glBindVertexArray(_vertexArray);

			glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(openGL::Vertex) * _vertices.size(), &_vertices[0], GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * _indices.size(), &_indices[0], GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(openGL::Vertex), (void*)offsetof(openGL::Vertex, Position));
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(openGL::Vertex), (void*)offsetof(openGL::Vertex, Normal));
			glEnableVertexAttribArray(1);

			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(openGL::Vertex), (void*)offsetof(openGL::Vertex, UV));
			glEnableVertexAttribArray(2);

			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(openGL::Vertex), (void*)offsetof(openGL::Vertex, Tangent));
			glEnableVertexAttribArray(3);

			glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(openGL::Vertex), (void*)offsetof(openGL::Vertex, Bitangent));
			glEnableVertexAttribArray(4);

			glBindVertexArray(0);
		}

		void Draw() const
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, _textureId);

			glBindVertexArray(_vertexArray);
			glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, nullptr);
			glBindVertexArray(0);
		}

		bool normalMapping = true;

		void ToggleNormalMapping()
		{
			glBindVertexArray(_vertexArray);

			normalMapping = !normalMapping;
			if (normalMapping)
			{
				glEnableVertexAttribArray(1);
			}
			else
			{
				glDisableVertexAttribArray(1);
			}

			glBindVertexArray(0);
		}

	protected:
		int _terrainWidth, _terrainHeight;
		int _heightmapWidth, _heightmapHeight;
		GLuint _vertexBuffer, _indexBuffer, _vertexArray;
		GLint _textureId;

		std::vector<openGL::Vertex> _vertices;
		std::vector<GLuint> _indices;
	};
}

#endif //!OPENGL_TERRAIN_INCLUDED