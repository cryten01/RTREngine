#include "JSONMapper.h"



JSONMapper::JSONMapper()
{
}


JSONMapper::~JSONMapper()
{
}

Document JSONMapper::loadDom(const std::string & jFilePath)
{
	// Read in json file
	std::string jsonString = Utils::readInFile(jFilePath);
	const char* json = jsonString.c_str();

	// Parse json into document object as a DOM tree
	Document dom;
	dom.Parse(json);

	// Check if root is an object
	assert(dom.IsObject());


	return dom;
}


float JSONMapper::getFloat(const Value & jFloat)
{
	return jFloat.GetFloat();
}


std::string JSONMapper::getString(const Value& jString)
{
	return jString.GetString();
}



MeshData JSONMapper::getMeshData(const Value & jType, const Value & jVecArray)
{
	std::string type = getString(jType);
	glm::vec3 dimensions = getVec3(jVecArray);

	if (type == "cube")
	{
		return Mesh::createCubeGeometry(dimensions[0], dimensions[1], dimensions[2]);
	}
	else if (type == "sphere")
	{
		return Mesh::createSphereGeometry(dimensions[0], dimensions[1], dimensions[2]);
	}
	else
	{
		return Mesh::createCylinderGeometry(dimensions[0], dimensions[1], dimensions[2]);
	}
}

TextureType JSONMapper::getTextureType(const Value& jType)
{
	std::string type = getString(jType);

	if (type == "diffuse")
		return TEX_DIFFUSE;
	else
		return TEX_SPECULAR;
}

MaterialType JSONMapper::getMaterialType(const Value & jType)
{
	std::string type = getString(jType);

	if (type == "color")
		return DIFFUSE;
	else
		return TEXTURE;
}

glm::vec3 JSONMapper::getVec3(const Value& jVector)
{
	return glm::vec3(getFloat(jVector[0]), getFloat(jVector[1]), getFloat(jVector[2]));
}

Texture& JSONMapper::getTexture(std::map<std::string, Texture>& map, const Value& jKey)
{
	std::string keyString = getString(jKey);
	
	return map.find(keyString)->second;
};

std::shared_ptr<Material> JSONMapper::getMaterial(std::map<std::string, std::shared_ptr<Material>>& map, const Value & jKey)
{
	std::string keyString = getString(jKey);

	return map.find(keyString)->second;
}
