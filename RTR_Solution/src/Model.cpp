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
		SceneObject* mesh = new SceneObject(processSceneObj(aMesh, scene));

		// Create an entry reference for the mesh
		_sceneObjEntries.push_back(mesh);
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
	for (int i = 0; i < _sceneObjEntries.size(); ++i) {
		delete _sceneObjEntries.at(i);
	}
	_sceneObjEntries.clear();
}


/**
*	Loads the specified aiMesh
**/
SceneObject Model::processSceneObj(aiMesh *mesh, const aiScene *scene)
{
	SceneObject sceneObj(_shader, glm::mat4(1));

	MeshData meshData = loadMesh(mesh);
	std::shared_ptr<Material> material = loadMaterial(mesh, scene);
	std::shared_ptr<Mesh> meshObj = std::make_shared<Mesh>(meshData, material);

	sceneObj.setMesh(meshObj);

	return sceneObj;
}


/**
*	Loads the specified geometry (similar to Mesh::createSphereGeometry())
**/
MeshData Model::loadMesh(aiMesh* mesh)
{
	MeshData data;

	if (mesh->HasPositions() && mesh->HasNormals() && mesh->HasTextureCoords(0))
	{
		for (int i = 0; i < mesh->mNumVertices; i++) {
			glm::vec3 vertexPos;
			vertexPos.x = mesh->mVertices[i].x;
			vertexPos.y = mesh->mVertices[i].y;
			vertexPos.z = mesh->mVertices[i].z;
			data.positions.push_back(vertexPos);

			glm::vec3 normal;
			normal.x = mesh->mNormals[i].x;
			normal.y = mesh->mNormals[i].y;
			normal.z = mesh->mNormals[i].z;
			data.normals.push_back(normal);

			if (mesh->HasTextureCoords(0))
			{
				glm::vec2 uv;
				uv.x = mesh->mTextureCoords[0][i].x;
				uv.y = mesh->mTextureCoords[0][i].y;
				data.uv.push_back(uv);
			}
		}
	}

	if (mesh->HasFaces())
	{
		for (int i = 0; i < mesh->mNumFaces; i++) {
			data.indices.push_back(mesh->mFaces[i].mIndices[0]);
			data.indices.push_back(mesh->mFaces[i].mIndices[1]);
			data.indices.push_back(mesh->mFaces[i].mIndices[2]);
		}
	}

	return std::move(data);
}


/**
*	Loads the material of a mesh with its properties such as colors and texture maps
**/
std::shared_ptr<Material> Model::loadMaterial(aiMesh* mesh, const aiScene *scene)
{
	std::shared_ptr<Material> material;

	// Retrieve material if mesh uses one
	if (mesh->mMaterialIndex >= 0)
	{
		// Get material from the scenes material array
		aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<Texture> allMaps;
		std::vector<Texture> diffuseMaps = loadTextures(mat, aiTextureType_DIFFUSE, TEX_DIFFUSE);
		std::vector<Texture> specularMaps = loadTextures(mat, aiTextureType_SPECULAR, TEX_SPECULAR);

		allMaps.insert(allMaps.end(), diffuseMaps.begin(), diffuseMaps.end());
		allMaps.insert(allMaps.end(), specularMaps.begin(), specularMaps.end());

		//Texture leatherTexture("../assets/textures/leather.jpg", TEX_DIFFUSE);
		material = std::make_shared<TextureMaterial>(_shader, glm::vec3(0.0f, 0.7f, 0.0f), 1.0f, allMaps);

		// Set material state
		material->getState() = DIFFUSE;
	}

	return material;
}


/**
*	Renders all allMaps of a specific type of a material
**/
std::vector<Texture> Model::loadTextures(aiMaterial* mat, aiTextureType aType, TextureType type)
{
	std::vector<Texture> textureEntries;

	// Loop over all textures stored of the given type of this material (diffuse, specular)
	for (unsigned int i = 0; i < mat->GetTextureCount(aType); i++)
	{
		// Retrieve local texture fileName and store it in aiString
		aiString texName;
		mat->GetTexture(aType, i, &texName);

		// If texture was loaded before, skip texture generation and use the one store in _textures_loaded
		bool skip = false;
		for (unsigned int j = 0; j < _textures_loaded.size(); j++)
		{
			// Compare strings (0 means identical)
			if (std::strcmp(_textures_loaded[j]._path.data(), texName.C_Str()) == 0)
			{
				// Push back already loaded texture
				textureEntries.push_back(_textures_loaded.at(j));
				skip = true;
				break;
			}
		}

		// If texture was not loaded before, create it
		if (!skip)
		{
			// Create full texture path
			std::string texPath = _directory + '/' + texName.C_Str();

			// Create texture
			Texture texture(texPath.c_str(), type);

			// Store texture as material texture as well as loaded texture
			textureEntries.push_back(texture);
			_textures_loaded.push_back(texture);
		}
	}

	return textureEntries;
}


/**
*	Renders all loaded meshes
**/
void Model::render() {
	for (GLuint i = 0; i < _sceneObjEntries.size(); i++) {

		_sceneObjEntries.at(i)->render();
	}
}