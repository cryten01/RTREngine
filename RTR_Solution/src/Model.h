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
	std::vector<Mesh*> _meshEntries;
	std::shared_ptr<Shader> _shader;							// The materialShader reference that is necessary for rendering the model
	std::vector<Texture*> _textures_loaded;		// Contains textures that are already loaded from the model

	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);

	GeometryData loadGeometry(aiMesh* mesh);
	std::vector<std::shared_ptr<Texture>> loadTextures(aiMaterial* aiMaterial, aiTextureType aiType, texType type, std::shared_ptr<TextureMaterial> material);

public:
	 Model(const std::string modelFilePath, std::shared_ptr<Shader> materialShader);
	~Model();

	void draw(std::shared_ptr<Shader> shader);
};

