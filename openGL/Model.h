#ifndef OPENGL_MODEL_INCLUDED
#define OPENGL_MODEL_INCLUDED

#include <vector>
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/material.h"
#include "Program.h"
#include "Mesh.h"

namespace openGL
{
	struct Model
	{
		std::vector<openGL::Mesh>& Meshes() { return _meshes; }

		explicit Model(openGL::Program* shaderProgram, std::string filepath)
			: _program(shaderProgram)
		{
			this->LoadModel(filepath);
		}

		void Draw() const
		{
			GLuint count = _meshes.size();
			if (count != 0)
			{
				_program->Bind();

				for (GLuint i = 0; i < count; i++)
				{
					_meshes[i].Draw();
				}

				_program->Unbind();
			}
		}

	protected:
		openGL::Program* _program;
		std::vector<openGL::Mesh> _meshes;
		std::string _directory;

	private:
		void LoadModel(std::string filepath)
		{
			Assimp::Importer importer;
			const aiScene* scene = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs);
			if (!scene || !scene->mRootNode || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE)
			{
				std::cout << "Assimp error: " << importer.GetErrorString();
				throw std::exception(importer.GetErrorString());
			}

			_directory = filepath.substr(0, filepath.find_last_of("\\"));
			this->ProcessNode(scene->mRootNode, scene);
		}

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
				vertex.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
				vertex.UV = mesh->HasTextureCoords(0)
					? glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y)
					: glm::vec2(0.0f, 0.0f);
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
			if (mesh->mMaterialIndex > -1)
			{
				aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
				std::vector<Texture> diffuseMaps = LoadTexture(material, aiTextureType_DIFFUSE, "texture_diffuse");
				std::vector<Texture> specularMaps = LoadTexture(material, aiTextureType_SPECULAR, "texture_specular");
				textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
				textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
			}

			return openGL::Mesh(vertices, indices, textures);
		}

		std::vector<openGL::Texture> loaded_textures;

		std::vector<openGL::Texture> LoadTexture(aiMaterial* material, aiTextureType type, std::string typeName)
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
					texture.Type = typeName;
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