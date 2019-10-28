#pragma once

#include "Utils.h"
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model
{
private:
	std::string _directory;
	std::vector<Mesh> _meshes;

	void loadModel(const std::string& modelFilePath);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* aiMaterial, aiTextureType aiType, std::string typeName);

public:
	 Model(const std::string& modelFilePath);
	~Model();

	void draw();
};

