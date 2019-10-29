#include "Model.h"



Model::Model(const std::string modelFilePath, std::shared_ptr<Shader> materialShader)
	: _materialShader(materialShader)
{
	loadModel(modelFilePath);
}


Model::~Model()
{
}


void Model::loadModel(const std::string modelFilePath) 
{
	/** Read in object file via Assimp
	/ aiProcess_Triangulate transforms all primitive shapes into triangles
	/ aiProcess_FlipUVs flips the texture coordinates on the y-axis where necessary 
	*/
	Assimp::Importer import;
	const aiScene *scene = import.ReadFile(modelFilePath, aiProcess_Triangulate | aiProcess_FlipUVs);

	/** Check for errors 
	/ if scene/root node are not NULL
	/ if data is incomplete via flags
	*/
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}

	// Retrieve the directory path of the given file path
	_directory = modelFilePath.substr(0, modelFilePath.find_last_of('/'));

	// Recursively process all nodes starting with the root node
	processNode(scene->mRootNode, scene);
}


void Model::processNode(aiNode *node, const aiScene *scene)
{
	// Process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		// Check mesh index and retrieve corresponding mesh
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
	
		// Get the actual mesh object from processMesh()
		Mesh meshObj = processMesh(mesh, scene);

		// Stores the meshObj
		_meshes.push_back(meshObj);
	}

	// Once all meshes are processed recursively do the same for each children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}


Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
	// Load GeometryData from mesh
	GeometryData data = loadMeshGeometry(mesh);
	std::shared_ptr<TextureMaterial> meshMaterial = std::make_shared<TextureMaterial>(_materialShader, glm::vec3(1.0f, 0.0f, 0.0f)); // Needs shader!

	// Check if mesh contains material or not (responsible for material creation of Mesh)
	if (mesh->mMaterialIndex >= 0)
	{
		// Retrieve material
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

		// Get diffuse and specular maps of mesh
		std::vector<std::shared_ptr<Texture>> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, texture_diffuse, meshMaterial);
		std::vector<std::shared_ptr<Texture>> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, texture_specular, meshMaterial);

		// Insert diffuseMap textures first then specularMaps into textures of this material
		meshMaterial->_textures.insert(meshMaterial->_textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		meshMaterial->_textures.insert(meshMaterial->_textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return Mesh(glm::mat4(1.0f), data, meshMaterial);
}


/** loadMeshGeometry similar to createSphereGeometry in Mesh.cpp */
GeometryData Model::loadMeshGeometry(aiMesh* mesh) 
{
	GeometryData data;

	/* Retrieve positions, normals, uvs for Mesh (one set per iteration) */
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		glm::vec3 posNormContainer;

		// Retrieve and push_back position
		posNormContainer.x = mesh->mVertices[i].x;
		posNormContainer.y = mesh->mVertices[i].y;
		posNormContainer.z = mesh->mVertices[i].z;
		data.positions.push_back(posNormContainer);

		// Retrieve and push_back normal vector
		posNormContainer.x = mesh->mNormals[i].x;
		posNormContainer.y = mesh->mNormals[i].y;
		posNormContainer.z = mesh->mNormals[i].z;
		data.normals.push_back(posNormContainer);

		// Retrieve and push_back uv coordinate (up to 8 different uv coordinates per vertex although only first relevant)
		// Checks if mesh contains uv coordinates
		if (mesh->mTextureCoords[0])
		{
			glm::vec2 uvContainer;

			uvContainer.x = mesh->mTextureCoords[0][i].x;
			uvContainer.y = mesh->mTextureCoords[0][i].y;
			data.uv.push_back(uvContainer);
		}
		// Place empty uv coordinate for completion reasons?
		else
		{
			data.uv.push_back(glm::vec2(0.0f, 0.0f));
		}
	}

	/* Loop over all faces in oder to retrieve all indices */
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		// Retrieve all indices of face
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			data.indices.push_back(face.mIndices[j]);
		}
	}

	return std::move(data);
}


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