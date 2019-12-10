#include "JSONMapper.h"


JMapper::JMapper(const Value& jObject)
	: _jObject(jObject)
{
}


JMapper::~JMapper()
{
}

Document JMapper::loadDom(const std::string & jFilePath)
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

void JMapper::checkObjectStructure(const Value& objectArray, std::vector<const char*> checkList)
{
	for (auto& object : objectArray.GetArray())
	{
		// Structure check
		for (const char* attribute : checkList)
		{
			assert(object.HasMember(attribute));
		}
	}
}


bool JMapper::getBool(const char* attribute) 
{
	return 	_jObject[attribute].GetBool();
};

float JMapper::getFloat(const char* attribute)
{
	return 	_jObject[attribute].GetFloat();
}


std::string JMapper::getString(const char* attribute)
{
	return 	_jObject[attribute].GetString();
}


glm::vec3 JMapper::getVec3(const char* attribute)
{
	float x = _jObject[attribute][0].GetFloat();
	float y = _jObject[attribute][1].GetFloat();
	float z = _jObject[attribute][2].GetFloat();

	return glm::vec3(x,y,z);
}


TextureType JMapper::getTextureType(const char* attribute)
{
	std::string type = getString(attribute);

	if (type == "diffuse")
		return TEX_DIFFUSE;
	else
		return TEX_SPECULAR;
}


MaterialType JMapper::getMaterialType(const char* attribute)
{
	std::string type = getString(attribute);

	if (type == "color")
		return DIFFUSE;
	else
		return TEXTURE;
}


MeshData JMapper::getMeshData(const char* jType, const char* array)
{
	std::string type = getString(jType);
	glm::vec3 dimensions = getVec3(array);

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