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
	//_directory = modelFilePath.substr(0, modelFilePath.find_last_of('/'));

	// Recursively process all nodes starting with the root node
	// processNode(scene->mRootNode, scene);

	for (int i = 0; i < scene->mNumMeshes; ++i) 
	{
		aiMesh* aMesh = scene->mMeshes[i];

		Mesh* mesh = new Mesh (loadMesh(aMesh));

		_meshEntries.push_back(mesh);
	}
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
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
	
		// Get the actual mesh object from processMesh()
		//Mesh meshObj = loadMesh(mesh, scene);

		// Stores the meshObj
		//_meshes.push_back(meshObj);
	}

	// Once all meshes are processed recursively do the same for each children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}



Model::~Model() 
{
	for (int i = 0; i < _meshEntries.size(); ++i) {
		delete _meshEntries.at(i);
	}
	_meshEntries.clear();
}


/**
*	Loads the specified aiMesh
**/
Mesh Model::loadMesh(aiMesh *mesh)
{
	GeometryData geometry = loadMeshGeometry(mesh);

	//// Check if mesh contains material or not (responsible for material creation of Mesh)
	//if (mesh->mMaterialIndex >= 0)
	//{
	//	// Retrieve material
	//	aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

	//	// Get diffuse and specular maps of mesh
	//	std::vector<std::shared_ptr<Texture>> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, texture_diffuse, meshMaterial);
	//	std::vector<std::shared_ptr<Texture>> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, texture_specular, meshMaterial);

	//	// Insert diffuseMap textures first then specularMaps into textures of this material
	//	meshMaterial->_textures.insert(meshMaterial->_textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	//	meshMaterial->_textures.insert(meshMaterial->_textures.end(), specularMaps.begin(), specularMaps.end());
	//}

	return Mesh(
		glm::mat4(1.0f), 
		geometry, 
		std::make_shared<Material>(_shader, glm::vec3(1.0f, 0.0f, 0.0f))
	);
}


/**
*	Renders all loaded meshes
**/
void Model::draw(std::shared_ptr<Shader> shader) {
	for (GLuint i = 0; i < _meshEntries.size(); i++) {
		_meshEntries.at(i)->draw(_shader);
	}
}


















/**
*	Loads the specified geometry. Similar to createSphereGeometry in Mesh.cpp
**/
GeometryData Model::loadMeshGeometry(aiMesh* mesh)
{
	GeometryData data;

	if (mesh->HasPositions() && mesh->HasNormals() && mesh->HasTextureCoords(0)) {
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
	}

	if (mesh->HasFaces()) {
		for (int i = 0; i < mesh->mNumFaces; ++i) {
			data.indices.push_back(mesh->mFaces[i].mIndices[0]);
			data.indices.push_back(mesh->mFaces[i].mIndices[1]);
			data.indices.push_back(mesh->mFaces[i].mIndices[2]);
		}
	}

	return std::move(data);
}


/**
*	Loads all textures of a certain material
**/
std::vector<std::shared_ptr<Texture>> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, texType typeName, std::shared_ptr<TextureMaterial> material)
{
	std::vector<std::shared_ptr<Texture>> textureContainer;

	// Loop over the amount of textures stored
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		
		// Gets the texture of the given at location i
		mat->GetTexture(type, i, &str);

		// Check if texture was loaded before by looping through all textures that are already loaded
		bool skip = false;
		for (unsigned int j = 0; j < _textures_loaded.size(); j++)
		{
			// Compare strings (0 means identical)
			if (std::strcmp(_textures_loaded[j]->_path.data(), str.C_Str()) == 0)
			{
				// Store texture then skip every time the same texture gets loaded again
				material->_textures.push_back(_textures_loaded[j]);
				skip = true;
				break;
			}
		}

		// If texture hasn't been loaded already, load it
		if (!skip)
		{   
			// Create path of texture
			std::string texPath = _directory + "/" + str.C_Str();

			// Create texture (use c_str() only if string does not get changed or is used only once)
			std::shared_ptr<Texture> texture = std::make_shared<Texture>(texPath.c_str(), typeName);

			// Add texture as material texture and loaded texture
			material->_textures.push_back(texture);
			_textures_loaded.push_back(texture);
		}
	}
	return textureContainer;
}

