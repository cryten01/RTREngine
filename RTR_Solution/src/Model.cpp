#include "Model.h"



Model::Model(const std::string& modelFilePath)
{
	loadModel(modelFilePath);
}


Model::~Model()
{
}

void Model::loadModel(const std::string& modelFilePath) 
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

	//processNode(scene->mRootNode, scene);
}