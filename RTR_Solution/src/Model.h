#pragma once
#include "Utils.h"
#include "SceneObject.h"
#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace RTREngine
{
	class Model
	{
	private:
		std::string _directory;						// Directory of the model
		std::shared_ptr<Shader> _shader;			// The shader thats needed in sceneObjects, meshes, materials, textures
		std::vector<Texture> _textures_loaded;		// Textures that are already loaded from the model

		void processNode(std::shared_ptr<SceneObject> target, aiNode* node, const aiScene* scene);

		std::shared_ptr<Mesh> loadMesh(aiMesh* aMesh, const aiScene* aScene);
		MeshData loadMeshData(aiMesh* aMesh);
		std::shared_ptr<Material> loadMaterial(aiMesh* aMesh, const aiScene *aScene);
		std::vector<Texture> loadTextures(aiMaterial *aMat, aiTextureType aType, TextureType type);


	public:
		Model();
		~Model();

		void load(std::shared_ptr<SceneObject> target, const std::string filePath);
	};
}