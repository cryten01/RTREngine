#pragma once

#include "Utils.h"
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model
{
private:
	std::string _directory;						// Directory of the model files
	std::shared_ptr<Shader> _shader;
	std::vector<Mesh*> _meshEntries;			// The models meshes
	std::vector<Texture*> _textures_loaded;		// Contains textures that are already loaded from the model

	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);

	GeometryData loadGeometry(aiMesh* mesh);
	void loadMaterial(aiMesh* mesh, const aiScene *scene);
	void loadTextures(aiMaterial *mat, aiTextureType type, std::shared_ptr<TextureMaterial> material);

public:
	 Model(const std::string modelFilePath, std::shared_ptr<Shader> materialShader);
	~Model();

	void render();
};

