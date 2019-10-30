#pragma once

#include "Utils.h"
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model
{
private:
	std::string _directory;										// The directory of the model files
	std::vector<Mesh> _meshes;
	std::shared_ptr<Shader> _materialShader;					// The materialShader reference that is necessary for rendering the model
	std::vector<std::shared_ptr<Texture>> _textures_loaded;		// Contains textures that are already loaded from the model

	void loadModel(std::string modelFilePath);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	GeometryData loadMeshGeometry(aiMesh* mesh);
	std::vector<std::shared_ptr<Texture>> loadMaterialTextures(aiMaterial* aiMaterial, aiTextureType aiType, texType type, std::shared_ptr<TextureMaterial> material);

public:
	GeometryData debugData;

	 Model(const std::string modelFilePath, std::shared_ptr<Shader> materialShader);
	~Model();

	void draw();
};

