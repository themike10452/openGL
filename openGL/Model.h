#ifndef OPENGL_MODEL_INCLUDED
#define OPENGL_MODEL_INCLUDED

#include "Common.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/material.h"
#include "Program.h"
#include "Mesh.h"

#include <vector>

namespace openGL
{
	struct Model
	{
		std::vector<openGL::Mesh>& Meshes() { return _meshes; }

		Model()
		{
		}

		explicit Model(std::string filepath)
		{
			this->LoadModel(filepath);
		}

		void LoadModel(std::string filepath) throw(std::exception)
		{
			Assimp::Importer importer;
			GLuint flags = aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_CalcTangentSpace;
			const aiScene* scene = importer.ReadFile(filepath, flags);
			if (!scene || !scene->mRootNode || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE)
			{
				std::cout << "Assimp error: " << importer.GetErrorString();
				throw std::exception(importer.GetErrorString());
			}

			_directory = filepath.substr(0, filepath.find_last_of("\\")) + "\\";
			this->ProcessNode(scene->mRootNode, scene);
		}

		void Draw(openGL::Program* program) const
		{
			GLuint count = _meshes.size();
			if (count != 0)
			{
				for (GLuint i = 0; i < count; i++)
				{
					_meshes[i].Draw(program);
				}
			}
		}

	private:
		std::vector<openGL::Mesh> _meshes;
		std::vector<openGL::Texture> loaded_textures;
		std::string _directory;

		void ProcessNode(aiNode* node, const aiScene* scene)
		{
			for (GLuint i = 0; i < node->mNumMeshes; i++)
			{
				_meshes.push_back(this->ProcessMesh(scene->mMeshes[node->mMeshes[i]], scene));
			}

			for (GLuint i = 0; i < node->mNumChildren; i++)
			{
				this->ProcessNode(node->mChildren[i], scene);
			}
		}

		openGL::Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene)
		{
			std::vector<openGL::Vertex> vertices;
			std::vector<openGL::Texture> textures;
			std::vector<GLuint> indices;

			//process vertices
			for (GLuint i = 0; i < mesh->mNumVertices; i++)
			{
				openGL::Vertex vertex;
				vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

				if (mesh->HasNormals())
					vertex.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

				if (mesh->mTextureCoords[0])
					vertex.UV = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
				else
					vertex.UV = glm::vec2(0.0f, 0.0f);

				if (mesh->HasTangentsAndBitangents())
				{
					vertex.Tangent.x = mesh->mTangents[i].x;
					vertex.Tangent.y = mesh->mTangents[i].y;
					vertex.Tangent.z = mesh->mTangents[i].z;
					vertex.Bitangent.x = mesh->mBitangents[i].x;
					vertex.Bitangent.y = mesh->mBitangents[i].y;
					vertex.Bitangent.z = mesh->mBitangents[i].z;
				}

				vertices.push_back(vertex);
			}

			//process indices
			for (GLuint i = 0; i < mesh->mNumFaces; i++)
			{
				aiFace face = mesh->mFaces[i];
				for (GLuint j = 0; j < face.mNumIndices; j++)
				{
					indices.push_back(face.mIndices[j]);
				}
			}

			//process textures
			if (mesh->mMaterialIndex >= 0)
			{
				aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
				std::vector<Texture> diffuseMaps = LoadTexture(material, aiTextureType_DIFFUSE);
				textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
				std::vector<Texture> specularMaps = LoadTexture(material, aiTextureType_SPECULAR);
				textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
				std::vector<Texture> normalMaps = LoadTexture(material, aiTextureType_NORMALS);
				textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
				std::vector<Texture> heightMaps = LoadTexture(material, aiTextureType_HEIGHT);
				textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
			}

			return openGL::Mesh(vertices, indices, textures);
		}

		std::vector<openGL::Texture> LoadTexture(aiMaterial* material, aiTextureType type)
		{
			std::vector<openGL::Texture> textures;
			GLuint count = material->GetTextureCount(type);

			for (GLuint i = 0; i < count; i++)
			{
				aiString str;
				material->GetTexture(type, i, &str);
				bool skip = false;
				for (openGL::Texture texture : loaded_textures)
				{
					if (texture.Path == str)
					{
						textures.push_back(texture);
						skip = true;
						break;
					}
				}

				if (!skip)
				{
					openGL::Texture texture;
					texture.Id = Utils::TextureFromFile(str.C_Str(), _directory);
					texture.Type = type;
					texture.Path = str;
					textures.push_back(texture);
					loaded_textures.push_back(texture);
				}
			}
			return textures;
		}
	};
}

#endif //!OPENGL_MODEL_INCLUDED