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

	Texture& lookupTexture(const char* attribute, std::map<std::string, Texture>& map);
	std::shared_ptr<Material> lookupMaterial(const char* attribute, std::map<std::string, std::shared_ptr<Material>>& map);

};

