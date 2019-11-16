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
	std::vector<std::shared_ptr<SceneObject>> _sceneObjEntries;	// The meshes of the model
	std::vector<Texture> _textures_loaded;		// Textures that are already loaded from the model


	std::shared_ptr<SceneObject> processNode(aiNode* node, const aiScene* scene);

	std::shared_ptr<Mesh> loadMesh(aiMesh* aMesh, const aiScene* aScene);
	MeshData loadMeshData(aiMesh* aMesh);
	std::shared_ptr<Material> loadMaterial(aiMesh* aMesh, const aiScene *aScene);
	std::vector<Texture> loadTextures(aiMaterial *aMat, aiTextureType aType, TextureType type);


public:
	 Model(const std::string modelFilePath, std::shared_ptr<Shader> materialShader);
	~Model();

	std::shared_ptr<SceneObject> loadIntoSceneObj(std::shared_ptr<SceneObject> target, const std::string filePath);
	void render();
};

