#pragma once
#include "Utils.h"
#include "Texture.h"
#include "Material.h"
#include "Mesh.h"



struct JTexture
{
	std::string name;
	std::string format;
	TextureType texType;
};


/**
*	A mapper for converting JSON attribute values to (interpreted) C++ values 
*	This mapper extends the rapidJSON library
**/
class JMapper
{
private:
	const Value& _jObject;

public:
	 JMapper(const Value& jObject);
	~JMapper();

	// Helper methods
	static Document loadDom(const std::string& jFilePath);
	static void checkObjectStructure(const Value& objectArray, std::vector<const char*> checkList);

	// Basic types (JSON)
	bool getBool(const char* attribute);
	float getFloat(const char* attribute);
	std::string getString(const char* attribute);

	// Extended types (C++)
	TextureType getTextureType(const char* attribute);
	MaterialType getMaterialType(const char* attribute);
	glm::vec3 getVec3(const char* attribute);
	MeshData getMeshData(const char* type, const char* array);

	template<typename T>
	T getEnum(const char* attribute) 
	{
		// Loop through enum

		// Return correct enum
	}

	template<typename T>
	T lookup(std::map<std::string, T>& map, const char* jAttributeName)
	{
		std::string key = getString(jAttributeName);
		return map.find(key)->second;
	}

	template<typename T>
	void insert(std::map<std::string, T>& map, std::string key, T value)
	{
		map.insert(std::pair<std::string, T>(key, value));
	}
};


