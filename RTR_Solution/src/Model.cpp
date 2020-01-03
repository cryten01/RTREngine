#include "Model.h"



Model::Model()
{
}


Model::~Model()
{
}


/**
*	Loads the model into the target sceneObject
**/
void Model::load(std::shared_ptr<SceneObject> target, const std::string filePath)
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

	// Set default shader for each sceneObject (TODO!!)
	//this->_shader = target->getShader();

	// Recursively process all nodes starting with the root node
	processNode(target, scene->mRootNode, scene);
}


/**
*	Processes the specified node
**/
void Model::processNode(std::shared_ptr<SceneObject> target, aiNode *node, const aiScene *scene)
{
	// Process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		// Check mesh index and retrieve corresponding mesh
		aiMesh* aMesh = scene->mMeshes[node->mMeshes[i]];

		// Load mesh sceneObject
		std::shared_ptr<Mesh> mesh = loadMesh(aMesh, scene);

		// Add mesh to sceneObject (TODO!!!)
		//target->addComponent(mesh);
	}

	// Once all meshes are processed recursively do the same for each children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		// Create child scene object
		std::shared_ptr<SceneObject> child = std::make_shared<SceneObject>(glm::mat4(1));

		// Add child reference to target
		target->addChild(child);

		// Call for each children
		processNode(child, node->mChildren[i], scene);
	}
}


/**
*	Loads the mesh
**/
std::shared_ptr<Mesh> Model::loadMesh(aiMesh *aMesh, const aiScene *aScene)
{
	std::shared_ptr<Mesh> mesh;

	MeshData meshData = loadMeshData(aMesh);
	std::shared_ptr<Material> material = loadMaterial(aMesh, aScene);
	mesh = std::make_shared<Mesh>(meshData, material);

	return mesh;
}


/**
*	Loads the specified geometry (similar to Mesh::createSphereGeometry())
**/
MeshData Model::loadMeshData(aiMesh* aMesh)
{
	MeshData data;

	if (aMesh->HasPositions() && aMesh->HasNormals() && aMesh->HasTextureCoords(0))
	{
		for (int i = 0; i < aMesh->mNumVertices; i++) {
			glm::vec3 vertexPos;
			vertexPos.x = aMesh->mVertices[i].x;
			vertexPos.y = aMesh->mVertices[i].y;
			vertexPos.z = aMesh->mVertices[i].z;
			data.positions.push_back(vertexPos);

			glm::vec3 normal;
			normal.x = aMesh->mNormals[i].x;
			normal.y = aMesh->mNormals[i].y;
			normal.z = aMesh->mNormals[i].z;
			data.normals.push_back(normal);

			if (aMesh->HasTextureCoords(0))
			{
				glm::vec2 uv;
				uv.x = aMesh->mTextureCoords[0][i].x;
				uv.y = aMesh->mTextureCoords[0][i].y;
				data.uv.push_back(uv);
			}
		}
	}

	if (aMesh->HasFaces())
	{
		for (int i = 0; i < aMesh->mNumFaces; i++) {
			data.indices.push_back(aMesh->mFaces[i].mIndices[0]);
			data.indices.push_back(aMesh->mFaces[i].mIndices[1]);
			data.indices.push_back(aMesh->mFaces[i].mIndices[2]);
		}
	}

	return std::move(data);
}


/**
*	Loads the material of a mesh with its properties such as colors and texture maps
**/
std::shared_ptr<Material> Model::loadMaterial(aiMesh* aMesh, const aiScene *aScene)
{
	std::shared_ptr<Material> material;

	// Retrieve material if mesh uses one
	if (aMesh->mMaterialIndex >= 0)
	{
		// Get material from the scenes material array
		aiMaterial* mat = aScene->mMaterials[aMesh->mMaterialIndex];

		std::vector<Texture> allMaps;
		std::vector<Texture> diffuseMaps = loadTextures(mat, aiTextureType_DIFFUSE, TEX_DIFFUSE);
		std::vector<Texture> specularMaps = loadTextures(mat, aiTextureType_SPECULAR, TEX_SPECULAR);

		allMaps.insert(allMaps.end(), diffuseMaps.begin(), diffuseMaps.end());
		allMaps.insert(allMaps.end(), specularMaps.begin(), specularMaps.end());


		// Set mesh material to loaded textures if existent, otherwise create default material
		if (allMaps.size() > 0) 
		{
			//material = std::make_shared<TextureMaterial>(_shader, glm::vec3(0.0f, 0.7f, 0.0f), 1.0f, allMaps);
		}
		else 
		{
			material = std::make_shared<Material>(_shader, glm::vec3(0.2f, 0.4f, 0.8f), 1.0f, glm::vec3(1.0f, 0.0f, 1.0f));
		}
	}

	return material;
}


/**
*	Renders all allMaps of a specific type of a material
**/
std::vector<Texture> Model::loadTextures(aiMaterial* aMat, aiTextureType aType, TextureType type)
{
	std::vector<Texture> textureEntries;

	// Loop over all textures stored of the given type of this material (diffuse, specular)
	for (unsigned int i = 0; i < aMat->GetTextureCount(aType); i++)
	{
		// Retrieve local texture fileName and store it in aiString
		aiString texName;
		aMat->GetTexture(aType, i, &texName);

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