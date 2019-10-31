#include "Model.h"


/**
*	Model constructor
**/
Model::Model(const std::string filePath, std::shared_ptr<Shader> shader)
	: _shader(shader)
{
	/** Read in object file via Assimp
	*	aiProcess_Triangulate transforms all primitive shapes into triangles (prevents mesh holes)
	*	aiProcess_FlipUVs flips the texture coordinates on the y-axis where necessary
	**/
	Assimp::Importer import;
	const aiScene *scene = import.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);

	/** Check for errors
	*	if scene/root node are not NULL
	*	if data is incomplete via flags
	*/
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}

	// Retrieve the directory path of the given file path
	_directory = filePath.substr(0, filePath.find_last_of('/'));

	// Recursively process all nodes starting with the root node
	processNode(scene->mRootNode, scene);
}


/**
*	Processes the specified node
**/
void Model::processNode(aiNode *node, const aiScene *scene)
{
	// Process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		// Check mesh index and retrieve corresponding mesh
		aiMesh* aMesh = scene->mMeshes[node->mMeshes[i]];

		// Create the actual mesh object from processMesh()
		Mesh* mesh = new Mesh(processMesh(aMesh, scene));

		// Create an entry reference for the mesh
		_meshEntries.push_back(mesh);
	}

	// Once all meshes are processed recursively do the same for each children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}


/**
*	Clears all loaded MeshEntries
**/
Model::~Model()
{
	for (int i = 0; i < _meshEntries.size(); ++i) {
		delete _meshEntries.at(i);
	}
	_meshEntries.clear();

	for (int i = 0; i < _textures_loaded.size(); ++i) {
		delete _textures_loaded.at(i);
	}
	_textures_loaded.clear();
}


/**
*	Loads the specified aiMesh
**/
Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
	GeometryData geometry = loadGeometry(mesh);
	std::shared_ptr<Material> debugMaterial = std::make_shared<Material>(_shader, glm::vec3(1.0f, 0.0f, 1.0f));

	return Mesh(glm::mat4(1.0f), geometry, debugMaterial);
}


/**
*	Loads the specified geometry (similar to Mesh::createSphereGeometry())
**/
GeometryData Model::loadGeometry(aiMesh* mesh)
{
	GeometryData data;

	if (mesh->HasPositions() && mesh->HasNormals() && mesh->HasTextureCoords(0) && mesh->HasFaces())
	{
		for (int i = 0; i < mesh->mNumVertices; ++i) {
			glm::vec3 vertexPos;
			vertexPos.x = mesh->mVertices[i].x;
			vertexPos.y = mesh->mVertices[i].y;
			vertexPos.z = mesh->mVertices[i].z;
			data.positions.push_back(vertexPos);

			glm::vec2 uv;
			uv.x = mesh->mTextureCoords[0][i].x;
			uv.y = mesh->mTextureCoords[0][i].y;
			data.uv.push_back(uv);

			glm::vec3 normal;
			normal.x = mesh->mNormals[i].x;
			normal.y = mesh->mNormals[i].y;
			normal.z = mesh->mNormals[i].z;
			data.normals.push_back(normal);
		}

		for (int i = 0; i < mesh->mNumFaces; ++i) {
			data.indices.push_back(mesh->mFaces[i].mIndices[0]);
			data.indices.push_back(mesh->mFaces[i].mIndices[1]);
			data.indices.push_back(mesh->mFaces[i].mIndices[2]);
		}
	}

	return std::move(data);
}


/**
*	Loads the material of a mesh
**/
void Model::loadMaterial(aiMesh* mesh, const aiScene *scene)
{
	// Retrieve material if mesh uses one
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

		// For debugging only!
		//Texture* debugTexture1 = new Texture("../assets/textures/leather.jpg", TEX_DIFFUSE);
		//Texture* debugTexture2 = new Texture("../assets/textures/leather.jpg", TEX_SPECULAR);
		//debugMaterial->addTexture(debugTexture1);
		//debugMaterial->addTexture(debugTexture2);
	}
}


/**
*	Loads all textures of a certain material
**/
void Model::loadTextures(aiMaterial *mat, aiTextureType type, std::shared_ptr<TextureMaterial> material)
{
	//// Loop over the amount of textures stored of the given type
	//for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	//{
	//	// Gets the texture of the given at location i
	//	aiString str;
	//	mat->GetTexture(type, i, &str);

	//	// If texture was loaded before, skip it
	//	bool skip = false;
	//	for (unsigned int j = 0; j < _textures_loaded.size(); j++)
	//	{
	//		// Compare strings (0 means identical)
	//		if (std::strcmp(_textures_loaded[j]->_path.data(), str.C_Str()) == 0)
	//		{
	//			// Store texture then skip every time the same texture gets loaded again
	//			material->addTexture(_textures_loaded.at(j));
	//			skip = true;
	//			break;
	//		}
	//	}

	//	// If texture was not loaded before, load it
	//	if (!skip)
	//	{
	//		// Create path of texture
	//		std::string texPath = _directory + "/" + str.C_Str();

	//		// Create texture (use c_str() only if string does not get changed or is used only once)
	//		TextureType type = (aiTextureType_DIFFUSE) ? TEX_DIFFUSE : TEX_SPECULAR;
	//		Texture* texture = new Texture(texPath.c_str(), type);

	//		// Add texture as material texture and loaded texture
	//		material->addTexture(texture);
	//		_textures_loaded.push_back(texture);
	//	}
	//}
}




/**
*	Renders all loaded meshes
**/
void Model::render() {
	for (GLuint i = 0; i < _meshEntries.size(); i++) {
		_meshEntries.at(i)->draw();
	}
}






