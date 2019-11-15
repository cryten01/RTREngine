#pragma once
#include "Utils.h"
#include "SceneObject.h"
#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model
{
private:
	std::string _directory;						// Directory of the model files
	std::shared_ptr<Shader> _shader;			// The shader thats needed for the material
	std::vector<SceneObject*> _sceneObjEntries;			// The meshes of the model
	std::vector<Texture> _textures_loaded;		// Textures that are already loaded from the model

	void processNode(aiNode* node, const aiScene* scene);
	SceneObject processSceneObj(aiMesh* mesh, const aiScene* scene);

	MeshData loadMesh(aiMesh* mesh);
	std::shared_ptr<Material> loadMaterial(aiMesh* mesh, const aiScene *scene);
	std::vector<Texture> loadTextures(aiMaterial *mat, aiTextureType aType, TextureType type);

public:
	 Model(const std::string modelFilePath, std::shared_ptr<Shader> materialShader);
	~Model();

	void render();
};

