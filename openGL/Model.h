#ifndef OPENGL_MODEL_INCLUDED
#define OPENGL_MODEL_INCLUDED

#include "Common.h"
#include "assimp/scene.h"
#include "assimp/material.h"
#include "Program.h"
#include "Mesh.h"

#include <vector>

namespace openGL
{
	struct Model
	{
		/// Getters
		std::vector<openGL::Mesh>& Meshes();
		/// -------

		Model();

		explicit Model(std::string filepath);

		void LoadModel(std::string filepath);

		void Draw(openGL::Program* program) const;

	private:
		std::vector<openGL::Mesh> _meshes;
		std::vector<openGL::Texture> loaded_textures;
		std::string _directory;

		void ProcessNode(aiNode* node, const aiScene* scene);

		openGL::Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);

		std::vector<openGL::Texture> LoadTexture(aiMaterial* material, aiTextureType type);
	};
}

#endif //!OPENGL_MODEL_INCLUDED